#include "Motro_Control.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "tim.h"
#include "lv_app_motro_control.h"
#include "Arm.h"
#include "lv_app_arm.h"
Motro_Control_ Motro_Control;
struct Queue Motro_Msg_Queue;

/*通讯控制*/
void Motro_UART_Queue(struct Queue* queue, bool sign)
{
	HAL_TIM_Base_Stop_IT(&htim4);

	if(sign)
	{
		QueuePop(queue, &Motro_Control.UART);
		Motro_Control.UART.times = 0;
	}
	HAL_UART_Transmit_DMA(Motro_Control.UART.huart, Motro_Control.UART.data, Motro_Control.UART.Size);
	TIM4->CNT = 0;
	HAL_TIM_Base_Start_IT(&htim4);
}

static void Motro_UART_Next_Queue()
{
	HAL_TIM_Base_Stop_IT(&htim4);
	if(Motro_Msg_Queue.size == 0)
	{
		Motro_Control.uart_finish_sign = true;
	}
	else
	{
		Motro_UART_Queue(&Motro_Msg_Queue, true);
	}
}

void Motro_UART_Timeout()
{
	lv_obj_Motro_Control.Control_Timeout.times++;
	lv_obj_Motro_Control.Control_Error_Only_Once.times++;
	Motro_UART_Next_Queue();
}

static void Motro_IDLE_IT_Process(Information_Inquire_ * Information, uint8_t Address, uint8_t Receive[], float (*Motro_Info_Calculate)(uint8_t Address, uint8_t data[]))
{
	uint8_t ID = Address - 1;
	Information->Value[ID] = Motro_Info_Calculate(Address, Receive);
	Information->Done_sign[ID] = true;
}

void Motro_IDLE_IT(uint8_t rx_len)
{
	uint8_t Address =  Motro_Control.UART.data[0];
	uint8_t *Receive = Motro_Control.UART.receive;
	uint8_t rx_length_set;
	bool Receive_Verify_Sign;
	Information_Inquire_ *Information;
	float (*Motro_Info_Calculate)(uint8_t Address, uint8_t *data);

	if(Motor_Position_Done(rx_len, Receive) == true)
	{
		return;
	}
	
	switch(Motro_Control.UART.receive_type)
	{
		/*普通控制*/
		case Motro_Control_Nomal_Sign:
			rx_length_set = 3;
			Receive_Verify_Sign = true;
			break;
		/*位置查询*/
		case Motro_Position_Check_Sign:
			rx_length_set = 6;
			Receive_Verify_Sign = false;
			Information = &Motro_Control.Angle;
			Motro_Info_Calculate = Motro_Position_Calculate;
			break;
		/*编码器查询*/
		case Motro_Encoder_Check_Sign:
			rx_length_set = 4;
			Receive_Verify_Sign = false;
			Information = &Motro_Control.Encoder_Angle;
			Motro_Info_Calculate = Motro_Encoder_Calculate;
			break;
	}
	if(Motro_Control.UART.times < Motro_Control.feedback_error)
	{
		if(rx_len == rx_length_set)
		{
			if(Receive_Verify_Sign == true)
			{
				if(memcmp(Receive,Motro_Control.UART.Verify,3) == 0)
					goto Next;
				else
					goto Retry;
			}
			else
			{
				Motro_IDLE_IT_Process(Information, Address, Receive, Motro_Info_Calculate);
				goto Next;
			}
		}
		else
			goto Retry;
	}
	else
		goto Next;

	Retry:
	{
		Motro_Control.UART.times++;
		Motro_UART_Queue(&Motro_Msg_Queue,false);
		return;
	}

	Next:
	{
		if(Motro_Control.UART.times==0)
			lv_obj_Motro_Control.Control_Success_Only_Once.times++;
		else
			lv_obj_Motro_Control.Control_Error_Only_Once.times++;
		Motro_UART_Next_Queue();
		return;
	}
}

static void Motro_Transmit_Verify(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint8_t* pVerify, uint8_t receive_type)
{
	if(Motro_Control.feedback_sign == true)
	{
		if(Motro_Msg_Queue.size > 10)
		{
			QueueDestroy(&Motro_Msg_Queue);
			QueueInit(&Motro_Msg_Queue);
		}
		UART_ UART;
		UART.huart = huart;
		memcpy(UART.data, pData, Size);
		UART.Size = Size;
		if(pVerify)
			memcpy(UART.Verify, pVerify, 3);
		UART.receive_type = receive_type;
		QueuePush(&Motro_Msg_Queue, UART);
		lv_obj_Motro_Control.Control.times++;
		if(Motro_Control.uart_finish_sign == true)
		{
			Motro_Control.uart_finish_sign = false;
			Motro_UART_Queue(&Motro_Msg_Queue, true);
		}
	}
	else
	{
		HAL_UART_Transmit(huart,pData,Size,HAL_MAX_DELAY);
		HAL_Delay(1);
	}
}

/*电机基础控制*/
void Motor_Open(uint8_t Address)
{
	uint8_t receive_type[3]={Address,0x02,0x6B};
	uint8_t data[4]={Address,0xF3,0X01,0X6B};
	Motro_Transmit_Verify(&huart3, data, 4, receive_type, Motro_Control_Nomal_Sign);
}

void Motor_Close(uint8_t Address)
{
	uint8_t receive_type[3]={Address,0x02,0x6B};
	uint8_t data[4]={Address,0xF3,0X00,0X6B};
	Motro_Transmit_Verify(&huart3, data, 4, receive_type, Motro_Control_Nomal_Sign);
}

void Motor_MStep_Set(uint8_t Address, uint8_t num)  //设置细分步数
{
	uint8_t data[4]={Address, 0x84, num, 0x6B};
	uint8_t receive_type[3]={Address,0x02,0x6B};
	Motro_Transmit_Verify(&huart3, data, 4, receive_type, Motro_Control_Nomal_Sign);
}

void Motor_Set_0(uint8_t Address)  //设置细分步数
{
	uint8_t data[4]={Address, 0x0A, 0x6D, 0x6B};
	uint8_t receive_type[3]={Address,0x02,0x6B};
	Motro_Transmit_Verify(&huart3, data, 4, receive_type, Motro_Control_Nomal_Sign);
}

/*速度控制*/
void Motor_Speed_Control(uint8_t Address, int16_t speed, uint8_t aacelerated)/*1为正方向，-1为反方向*/
{
	int sign=1;
	if(Address%2==0)	sign=-1;
	uint8_t data[6]={Address, 0xF6, 0x00, 0x00, aacelerated, 0x6B};
	if(sign*speed>0)	data[2]=0x10;
	else				data[2]=0x00;
	data[2]|=abs(speed)>>8;
	data[3]=abs(speed);

	uint8_t receive_type[3]={Address,0x02,0x6B};
	if(Address==0)
	{
		receive_type[0]=1;
	}

	Motro_Transmit_Verify(&huart3, data, 6, receive_type, Motro_Control_Nomal_Sign);
}

/*相对位置控制*/
void Motor_Relative_Position_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t accelerated, float angle, uint16_t Subdivision_Step, float Reduction_Ratio)
{
	uint8_t data[9]={Address,0xFD,0X00,0X00,accelerated,0X00,0X00,0X00,0X6B};
	if(direction * angle >0)	
	{
		data[2] = 0x10;
	}
	else
	{
		data[2] = 0x00;
	}					
	data[2] |= speed>>8;
	data[3] = speed;
	angle = fabs(angle);
	uint32_t pulse = (uint32_t)(angle * Subdivision_Step * Reduction_Ratio / 1.8f);
	data[5] = (uint8_t)(pulse >> 16);
	data[6] = (uint8_t)(pulse >> 8);
	data[7] = (uint8_t)(pulse);

	uint8_t receive_type[3]={Address,0x02,0x6B};
	Motro_Control.Angle_Control.Done_Num--;
	Motor_Position_TIM_Start(Address);
	Arm_Symbol_Set(Address, USR_SYMBOL_Underway);
	Motro_Transmit_Verify(&huart3,data,9,receive_type, Motro_Control_Nomal_Sign);
}
/*绝对位置控制*/
volatile float dataaaa;
void Motor_Absolute_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t accelerated, float angle, bool Angle_Check_Sign, uint16_t Subdivision_Step, float Reduction_Ratio)
{
    uint8_t ID = Address - 1;
    if(Motro_Control.Angle_Control.Done_sign[ID] == true)
    {
        Motro_Control.Angle_Control.Done_sign[ID] = false;
        if(Angle_Check_Sign)
        {
            uint8_t num = 0;
            uint8_t time_ms = 0;
            do{
                Motro_Position_Check(Address);
                do{
                    HAL_Delay(1);
                    time_ms++;
                }
                while(time_ms < 50 && Motro_Control.Angle.Done_sign[ID] != true);
                if(Motro_Control.Angle.Done_sign[ID])
                    break;
               num++;
            }while(num < 5);
        }
		Motro_Control.Angle_Control.Value_Pub[ID].angle_last = angle;
		dataaaa = angle - Motro_Control.Angle.Value[ID];
		Motor_Relative_Position_Control(Address, direction, speed, accelerated, dataaaa, Subdivision_Step, Reduction_Ratio);
    }
	else
	{
		Motro_Control.Angle_Control.Pub_sign[ID] = false;
		Motro_Control.Angle_Control.Value_Pub[ID].direction = direction;
		Motro_Control.Angle_Control.Value_Pub[ID].speed = speed;
		Motro_Control.Angle_Control.Value_Pub[ID].accelerated = accelerated;
		Motro_Control.Angle_Control.Value_Pub[ID].angle = angle;
		Motro_Control.Angle_Control.Value_Pub[ID].Angle_Check_Sign = Angle_Check_Sign;
		Motro_Control.Angle_Control.Value_Pub[ID].Subdivision_Step = Subdivision_Step;
		Motro_Control.Angle_Control.Value_Pub[ID].Reduction_Ratio = Reduction_Ratio;
	}
}
/*上电归零*/
void Motor_Absolute_Angle_To_0(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float Encoder_Angle_0, uint16_t Subdivision_Stepfloat, float Reduction_Ratio)
{
    uint8_t num = 0;
    uint8_t time_ms = 0;
    do{
        Motro_Encoder_Check(Address);
        do{
            HAL_Delay(1);
            time_ms++;
        }
        while(time_ms < 50 && (Motro_Control.Encoder_Angle.Done_sign[Address-1] != true));
        if(Motro_Control.Encoder_Angle.Done_sign[Address-1])
            break;
        num++;
    }while(num < 5);
    float Encoder_Angle = Motro_Control.Encoder_Angle.Value[Address-1];
	Motor_Absolute_Control(Address, direction, speed, aacelerated, (Encoder_Angle_0 - Encoder_Angle)/Reduction_Ratio, false, Subdivision_Stepfloat, Reduction_Ratio);
}
/*位置更新完成检测*/
bool Motor_Position_Done(uint8_t length, uint8_t data[])
{
	bool sign = false;
	if(Motro_Control.Angle_Control.Done_Num < Angle_Control_Num && length == 3)
	{
		uint8_t Address = data[0];
		if(Address == Arm_Rear_Address || Address == Arm_Front_Address || Address == Arm_Pedestal_Address)
		{
			uint8_t done[2] = {0x9F, 0x6B};
			uint8_t ID = Address - 1;
			if(memcmp(data+1, done, 2) == 0)
			{
				Motro_Control.Angle_Control.Done_sign[ID] = true;
				Motro_Control.Angle_Control.Done_Num++;
				Motor_Position_TIM_Stop(Address);
				Arm_Symbol_Set(Address, USR_SYMBOL_Complete);
				Motor_Position_Done_Ckeck(Address);
				sign = true;
			}
		}
	}
	return sign;
}
/*最终位置检测*/
static void Motor_Position_Done_Ckeck(uint8_t Address)
{
	uint8_t ID = Address - 1;
	if(Motro_Control.Angle_Control.Pub_sign[ID])
	{
		Motro_Position_Check(Address);
	}
	else
	{
		Motro_Control.Angle.Value[ID] = Motro_Control.Angle_Control.Value_Pub[ID].angle_last;
		Motro_Control.Angle_Control.Pub_sign[ID] = true;
		Motor_Absolute_Control(	Address,
								Motro_Control.Angle_Control.Value_Pub[ID].direction,
								Motro_Control.Angle_Control.Value_Pub[ID].speed,
								Motro_Control.Angle_Control.Value_Pub[ID].accelerated,
								Motro_Control.Angle_Control.Value_Pub[ID].angle,
								false,
								Motro_Control.Angle_Control.Value_Pub[ID].Subdivision_Step,
								Motro_Control.Angle_Control.Value_Pub[ID].Reduction_Ratio);
	}
}
/*位置更新超时处理*/
static void Motor_Position_TIM_Start(uint8_t Address)
{
	switch(Address)
	{
		case Arm_Rear_Address:
			TIM7->CNT = 0;
			HAL_TIM_Base_Start_IT(&htim7);
			break;
		case Arm_Front_Address:
			TIM9->CNT = 0;
			HAL_TIM_Base_Start_IT(&htim9);
			break;
		case Arm_Pedestal_Address:
			TIM11->CNT = 0;
			HAL_TIM_Base_Start_IT(&htim11);
			break;
	}
}

static void Motor_Position_TIM_Stop(uint8_t Address)
{
	switch(Address)
	{
		case Arm_Rear_Address:
			HAL_TIM_Base_Stop_IT(&htim7);
			break;
		case Arm_Front_Address:
			HAL_TIM_Base_Stop_IT(&htim9);
			break;
		case Arm_Pedestal_Address:
			HAL_TIM_Base_Stop_IT(&htim11);
			break;
	}
}

void Motor_Position_TIM_IT(TIM_HandleTypeDef *htim)
{
	if(htim == &htim7 || htim == &htim9 || htim == &htim11)
	{
		uint8_t Address;
		uint8_t ID;
		if(htim == &htim7)
			Address = Arm_Rear_Address;
		else if(htim == &htim9)
			Address = Arm_Front_Address;
		else if(htim == &htim11)
			Address = Arm_Pedestal_Address;
		ID = Address - 1;
		HAL_TIM_Base_Stop_IT(htim);
		Motro_Control.Angle_Control.Done_sign[ID] = true;
		Motro_Control.Angle_Control.Done_Num++;
		Motor_Position_Done_Ckeck(Address);
	}
}

/*角度查询*/
void Motro_Position_Check(uint8_t Address)
{
	uint8_t ID = Address - 1;
	Motro_Control.Angle.Value[ID] = 0;
	Motro_Control.Angle.Done_sign[ID] = false;
	uint8_t data[3] = {Address, 0x36, 0x6B};
	Motro_Transmit_Verify(&huart3, data, 3, NULL, Motro_Position_Check_Sign);
}

static float Motro_Position_Calculate(uint8_t Address, uint8_t data[])
{
	uint8_t ID = Address - 1;
	float Angle;
	int32_t Position = (int32_t)(	
									((int32_t)data[1] << 24) |
									((int32_t)data[2] << 16) |
									((int32_t)data[3] << 8 ) |
									((int32_t)data[4] << 0 )
								);
	
	lv_obj_t ** label = &lv_obj_ARM.Motor_Debug.label_Angle_Rear_Arm;
	Angle = (float)Position * 360.f / 65536.f / Motro_Control.Reduction_Ratio[ID];
	
	switch (Address)
	{
		case Arm_Rear_Address:
			label += 0;
			break;
		case Arm_Front_Address:
			label += 1;
			break;
		case Arm_Pedestal_Address:
			label += 2;
			break;
	}
	char string[20]={0};
	sprintf(string, "%5.1f", Angle);
	lv_label_set_text(*label, string);

	return Angle;
}

/*编码器查询*/
void Motro_Encoder_Check(uint8_t Address)
{
	uint8_t ID = Address - 1;
	Motro_Control.Encoder_Angle.Done_sign[ID] = false;
	uint8_t data[3] = {Address, 0x30, 0x6B};
	Motro_Transmit_Verify(&huart3, data, 3, NULL, Motro_Encoder_Check_Sign);
}

static float Motro_Encoder_Calculate(uint8_t Address, uint8_t data[])
{
	uint8_t ID = Address - 1;
	uint16_t Encoder = (uint16_t)(	
									((uint16_t)data[1] << 8) |
									((uint16_t)data[2])
								);
	float Angle = (float)Encoder * 360.f / 65536.f;
	
	Angle += Motro_Control.Encoder_Angle_Correction[ID];
	if(Angle >360)
		Angle -= 360;

	lv_obj_t ** label = &lv_obj_ARM.Motor_Debug.label_Angle_Rear_Arm;
	switch (Address)
	{
		case Arm_Rear_Address:
			label += 0;
			break;
		case Arm_Front_Address:
			label += 1;
			break;
		case Arm_Pedestal_Address:
			label += 2;
			break;
	}
	char string[20]={0};
	sprintf(string, "%5.1f", Angle);
	lv_label_set_text(*label, string);

	return Angle;
}
/*电机通讯初始化*/
void Motro_Control_Init()
{
	QueueInit(&Motro_Msg_Queue);
	
	for(uint8_t i=0; i<7; i++)
	{
		Motro_Control.Angle_Control.Done_sign[i] = true;
		Motro_Control.Angle.Value[i] = 0;
		Motro_Control.Angle_Control.Pub_sign[i] = true;
	}
	Motro_Control.Angle_Control.Done_Num = Angle_Control_Num;

	Motro_Control.Reduction_Ratio[Arm_Rear_Address-1] = Arm_Reduction_Ratio;
	Motro_Control.Reduction_Ratio[Arm_Front_Address-1] = Arm_Reduction_Ratio;
	Motro_Control.Reduction_Ratio[Arm_Pedestal_Address-1] = 1;

	Motro_Control.Encoder_Angle_Correction[Arm_Rear_Address-1] = Rear_Encoder_Angle_Correction;
	Motro_Control.Encoder_Angle_Correction[Arm_Front_Address-1] = Front_Encoder_Angle_Correction;
	Motro_Control.Encoder_Angle_Correction[Arm_Pedestal_Address-1] = Pedestal_Encoder_Angle_Correction;

	Motro_Control.feedback_error=5;
	Motro_Control.feedback_sign=true;
	Motro_Control.uart_finish_sign = true;
}