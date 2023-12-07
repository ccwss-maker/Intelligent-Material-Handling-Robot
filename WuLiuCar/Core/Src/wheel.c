#include <wheel.h>
#include <math.h> 
#include <stdlib.h>
#include <key.h>
#include <usart.h>
#include <imu.h>
#include <ble.h>

PID_	PID[2];
float SPD_FL,SPD_FR,SPD_BL,SPD_BR;
extern key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/
extern message_imu_ message_imu;
extern ble_data_ ble_data;
float w_pid;
uint8_t Wheel_PID_Sign=1; //1:启用PID控制 0：禁用PID控制
uint8_t Wheel_Close_once_sign=1;
///*车轮控制*/
//void Control_ALL(key_pressed_* key_pressed_p, mouse_information_ mouse_information, uint8_t page)
//{
//	if(page==4&&mouse_information.selected&&key_pressed_p->sign[1])
//	{
//		if(!mouse_information.mouse_2[0])
//		{
//			switch(mouse_information.mouse_2[1])
//			{
//				case 0:
//					Control(&PWM_FL,key_pressed_p,mouse_information.mouse_1,-2000,2000);
//					break;
//				case 1:
//					Control(&PWM_BL,key_pressed_p,mouse_information.mouse_1,-2000,2000);
//					break;
//			}
//		}
//		else
//		{
//			switch(mouse_information.mouse_2[1])
//			{
//				case 0:
//					Control(&PWM_FR,key_pressed_p,mouse_information.mouse_1,-2000,2000);
//					break;
//				case 1:
//					Control(&PWM_BR,key_pressed_p,mouse_information.mouse_1,-2000,2000);
//					break;
//			}
//		}
//	}
//}

//void Control(int* wheel_p, key_pressed_* key_pressed_p, uint8_t mode, int min, uint16_t max)
//{
//	if(key_pressed_p->T[0])	Control_add(wheel_p, &key_pressed_p->T[0], mode, max);
//	else if(key_pressed_p->B[0])	Control_sub(wheel_p, &key_pressed_p->B[0], mode, min);
//	else if(key_pressed_p->L[0])	Control_sub(wheel_p, &key_pressed_p->L[0], mode, min);
//	else if(key_pressed_p->R[0])	Control_add(wheel_p, &key_pressed_p->R[0], mode, max);
//}

//void Control_add(int* wheel_p, uint8_t* key, uint8_t mode, uint16_t max)
//{
//	switch(mode)
//	{
//		case 0:
//			switch(*key)
//			{
//				case 1:
//					if(*wheel_p+1 <= max)	*wheel_p +=1;
//					else								*wheel_p = max;
//					break;
//				case 2:
//					if(*wheel_p+1 <= max)	*wheel_p +=1;
//					else								*wheel_p = max;
//					break;
//				case 3:
//					if(*wheel_p+1 <= max)	*wheel_p +=1;
//					else								*wheel_p = max;
//					break;
//			}
//			break;
//		case 1:
//			if(*wheel_p+1 <= max)	*wheel_p +=10;
//			else								*wheel_p = max;
//			break;
//		case 2:
//			if(*wheel_p+1 <= max)	*wheel_p +=10;
//			else								*wheel_p = max;
//		case 3:
//			if(*wheel_p+1 <= max)	*wheel_p +=10;
//			else								*wheel_p = max;
//	}
//}
//void Control_sub(int* wheel_p, uint8_t* key, uint8_t mode, int min)
//{
//	switch(mode)
//	{
//		case 0:
//			switch(*key)
//			{
//				case 1:
//					if(*wheel_p-1>=min)	*wheel_p -=1;
//					else								*wheel_p = min;
//					break;
//				case 2:
//					if(*wheel_p-1>=min)	*wheel_p -=1;
//					else								*wheel_p = min;
//					break;
//				case 3:
//					if(*wheel_p-1>=min)	*wheel_p -=1;
//					else								*wheel_p = min;
//					break;
//			}
//			break;
//		case 1:
//			if(*wheel_p-1>=min)	*wheel_p -=10;
//			else								*wheel_p = min;
//			break;
//		case 2:
//			if(*wheel_p-10>=min)	*wheel_p -=10;
//			else								*wheel_p = min;
//			break;
//		case 3:
//			if(*wheel_p-100>=min)	*wheel_p -=10;
//			else								*wheel_p = min;
//	}
//}

void Control_SPD(mouse_information_ mouse_information, uint8_t page)
{
	if(page==2)
	{
		if(mouse_information.selected&&key_pressed.sign[1])
		{
			if(!mouse_information.mouse_2[0])
			{
				switch(mouse_information.mouse_2[1])
				{
					case 0:
						Control(&SPD_FL,mouse_information.mouse_1,-25,25);
						break;
					case 1:
						Control(&SPD_BL,mouse_information.mouse_1,-25,25);
						break;
				}
			}
			else
			{
				switch(mouse_information.mouse_2[1])
				{
					case 0:
						Control(&SPD_FR,mouse_information.mouse_1,-25,25);
						break;
					case 1:
						Control(&SPD_BR,mouse_information.mouse_1,-25,25);
						break;
				}
			}
		}
	}
	// Speed_pub(1,SPD_FL,10);
	// Speed_pub(2,SPD_FR,10);
	// Speed_pub(3,SPD_BL,10);
	// Speed_pub(4,SPD_BR,10);
}

void Control(float* wheel_p, uint8_t mode, int min, int max)
{
	if(key_pressed.T[0])	Control_add(wheel_p, key_pressed.T[0], mode, max);
	else if(key_pressed.B[0])	Control_sub(wheel_p, key_pressed.B[0], mode, min);
	else if(key_pressed.L[0])	Control_sub(wheel_p, key_pressed.L[0], mode, min);
	else if(key_pressed.R[0])	Control_add(wheel_p, key_pressed.R[0], mode, max);
}

void Control_add(float* wheel_p, uint8_t key, uint8_t mode, int max)
{
	switch(mode)
	{
		case 0:
			switch(key)
			{
				case 1:
					if(*wheel_p+PID_ADD_SUB_1 <= max)	*wheel_p +=PID_ADD_SUB_1;
					else								*wheel_p = max;
					break;
				case 2:
					if(*wheel_p+PID_ADD_SUB_2 <= max)	*wheel_p +=PID_ADD_SUB_2;
					else								*wheel_p = max;
					break;
				case 3:
					if(*wheel_p+PID_ADD_SUB_3 <= max)	*wheel_p +=PID_ADD_SUB_3;
					else								*wheel_p = max;
					break;
			}
			break;
		case 1:
			if(*wheel_p+PID_ADD_SUB_1 <= max)	*wheel_p +=PID_ADD_SUB_1;
			else								*wheel_p = max;
			break;
		case 2:
			if(*wheel_p+PID_ADD_SUB_2 <= max)	*wheel_p +=PID_ADD_SUB_2;
			else								*wheel_p = max;
		case 3:
			if(*wheel_p+PID_ADD_SUB_3 <= max)	*wheel_p +=PID_ADD_SUB_3;
			else								*wheel_p = max;
	}
}

void Control_sub(float* wheel_p, uint8_t key, uint8_t mode, int min)
{
	switch(mode)
	{
		case 0:
			switch(key)
			{
				case 1:
					if(*wheel_p-PID_ADD_SUB_1>=min)	*wheel_p -=PID_ADD_SUB_1;
					else								*wheel_p = min;
					break;
				case 2:
					if(*wheel_p-PID_ADD_SUB_2>=min)	*wheel_p -=PID_ADD_SUB_2;
					else								*wheel_p = min;
					break;
				case 3:
					if(*wheel_p-PID_ADD_SUB_3>=min)	*wheel_p -=PID_ADD_SUB_3;
					else								*wheel_p = min;
					break;
			}
			break;
		case 1:
			if(*wheel_p-PID_ADD_SUB_1>=min)	*wheel_p -=PID_ADD_SUB_1;
			else								*wheel_p = min;
			break;
		case 2:
			if(*wheel_p-PID_ADD_SUB_2>=min)	*wheel_p -=PID_ADD_SUB_2;
			else								*wheel_p = min;
			break;
		case 3:
			if(*wheel_p-PID_ADD_SUB_3>=min)	*wheel_p -=PID_ADD_SUB_3;
			else								*wheel_p = min;
	}
}

void Speed_pub(uint8_t address, int16_t speed, uint8_t aacelerated)/*1为正方向，-1为反方向*/
{
	int sign=1;
	if(address%2==0)	sign=-1;
	uint8_t data[6]={0};
	data[0]=address;
	data[1]=0xF6;
	if(sign*speed>0)	data[2]=0x10;
	else				data[2]=0x00;
	data[2]|=abs(speed)>>8;
	data[3]=abs(speed);
	data[4]=aacelerated;
	data[5]=0x6B;
	HAL_UART_Transmit(&huart1,data,6,HAL_MAX_DELAY);
	HAL_Delay(1);
}


void Control_Mecanum(float v_x, float v_y, float w, uint8_t aacelerated)
{
	wheel_open();
	if(Wheel_PID_Sign==1)
	{
		if(w==0)
		{
			extern float yaw;
			extern float www;
			w=PID_calc(&PID[1],message_imu.Yaw,0);//位置环
		}
		else
		{
			IMU_Z_Init();
		}
		w=PID_calc(&PID[0],message_imu.Wz,w);//速度环
	}
	SPD_FL = (v_y+v_x-w*(Car_a+Car_b))/Wheel_r;
	SPD_FR = (v_y-v_x+w*(Car_a+Car_b))/Wheel_r;
	SPD_BL = (v_y-v_x-w*(Car_a+Car_b))/Wheel_r;
	SPD_BR = (v_y+v_x+w*(Car_a+Car_b))/Wheel_r;
	Speed_pub(1,SPD_FL,aacelerated);
	Speed_pub(2,SPD_FR,aacelerated);
	Speed_pub(3,SPD_BL,aacelerated);
	Speed_pub(4,SPD_BR,aacelerated);
}

void wheel_open()
{
	uint8_t data[4]={0,0xF3,0X01,0X6B};
	HAL_UART_Transmit(&huart1,data,4,HAL_MAX_DELAY);
	HAL_Delay(1);
	
	Wheel_Close_once_sign=1;
}

void wheel_close()
{
	if(Wheel_Close_once_sign==1)
	{
		PID_clear(&PID[0]);
		PID_clear(&PID[1]);
		Control_Mecanum(0, 0, 0,255);
		Wheel_Close_once_sign=0;
	}
	if(Wheel_PID_Sign==1)
	{
		if(fabs(message_imu.Yaw)>PID[1].Dead_Zone||message_imu.W>5)
		{
			while(fabs(message_imu.Yaw)>PID[1].Dead_Zone||message_imu.W>5)
			{
				if(ble_data.break_sign==true)
				{
					ble_data.break_sign=false;
					PID_clear(&PID[0]);
					PID_clear(&PID[1]);
					return;
				}
				Control_Mecanum(0, 0, 0,255);
			}
			PID_clear(&PID[0]);
			PID_clear(&PID[1]);
			uint8_t data[4]={0,0xF3,0X00,0X6B};
			HAL_UART_Transmit(&huart1,data,4,HAL_MAX_DELAY);
			HAL_Delay(1);
		}
	}
	else
	{
		if(message_imu.W>5)
		{
			while(message_imu.W>5)
			{
				if(ble_data.break_sign==true)
				{
					ble_data.break_sign=false;
					return;
				}
				Control_Mecanum(0, 0, 0,255);
			}
			uint8_t data[4]={0,0xF3,0X00,0X6B};
			HAL_UART_Transmit(&huart1,data,4,HAL_MAX_DELAY);
			HAL_Delay(1);
		}
	}
}

void Set_MStep(uint8_t address, uint8_t num)  //设置细分步数
{
	uint8_t data[4]={0};
	data[0]=address;
	data[1]=0x84;
	data[2]=num;
	data[3]=0x6B;
	HAL_UART_Transmit(&huart1,data,4,HAL_MAX_DELAY);
	HAL_Delay(1);
}
void Wheel_Init()
{
	wheel_close();
	Set_MStep(0,0);
	PID_init(&PID[0], PID_DELTA, 1.9, 0.2, 4.5, 11, 11, 11, 0.1);    //速度
	PID_init(&PID[1], PID_POSITION, 0.025,0.0005,0.195,0.35,3,3,1); //位置
}

void Wheel_PID_BLE()
{
	uint16_t Yaw = (message_imu.Yaw+180)*10;
	uint8_t data[3]={0};
	data[0]=0x41;
	data[1]=(uint8_t)(Yaw>>8);
	data[2]=(uint8_t)Yaw;
	HAL_UART_Transmit(&huart6,(uint8_t*)data,3,HAL_MAX_DELAY);
}
