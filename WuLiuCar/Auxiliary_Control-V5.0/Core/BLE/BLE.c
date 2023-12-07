#include "BLE.h"
#include "tim.h"
#include "string.h"
#include "Master_Slave_Communication.h"

BLE_Pretreatment_Data_ BLE_Pretreatment_Data;
uint8_t BLE_Receive_Data[BLE_Uart_Buffer];

void BLE_Detection()
{
	if(Sensor_Msg.BLE_Data.Connected_Sign == false && HAL_GPIO_ReadPin(BLE_UART1_STATE_GPIO_Port, BLE_UART1_STATE_Pin) == GPIO_PIN_RESET)
	{
		Sensor_Msg.BLE_Data.Connected_Sign = true;
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
		HAL_UART_Receive_DMA(&huart1,BLE_Receive_Data,BLE_Uart_Buffer);
	}
	else if(Sensor_Msg.BLE_Data.Connected_Sign == true && HAL_GPIO_ReadPin(BLE_UART1_STATE_GPIO_Port, BLE_UART1_STATE_Pin) == GPIO_PIN_SET)
	{
		Sensor_Msg.BLE_Data.Connected_Sign = false;
		HAL_UART_DMAStop(&huart1);
		memset(&Sensor_Msg.BLE_Data.L_3, 0, 28);
	}
}

void BLE_Init()
{
	HAL_TIM_Base_Start_IT(&htim3);   //100ms未收到数据，清零
}

void BLE_Receive()
{
	TIM3->CNT = 0;
    if(BLE_Receive_Data[0] == 0x66)
    {
		memcpy(&BLE_Pretreatment_Data.head,BLE_Receive_Data,18);
		int16_t check=0;
		for(int i=0;i<7;i++)
			check^=*(&BLE_Pretreatment_Data.digital_data+i);
		if(check == BLE_Pretreatment_Data.check)
		{
			uint8_t msg=0x88;
			HAL_UART_Transmit(&huart1,&msg,1,10);
			for(int i=0;i<12;i++)
			{
				if((BLE_Pretreatment_Data.digital_data>>i&(int16_t)1)==1)	*(&(Sensor_Msg.BLE_Data.L_3)+i)=1;
				else											*(&(Sensor_Msg.BLE_Data.L_3)+i)=0;
			}
			memcpy(&Sensor_Msg.BLE_Data.digital_data, &BLE_Pretreatment_Data.digital_data, 16);
			if(Sensor_Msg.BLE_Data.L_2>2000)	Sensor_Msg.BLE_Data.L_2-=2000;
			else					Sensor_Msg.BLE_Data.L_2=0;
			if(Sensor_Msg.BLE_Data.R_2>2000)	Sensor_Msg.BLE_Data.R_2-=2000;
			else					Sensor_Msg.BLE_Data.R_2=0;
		}
    }
}

void BLE_Receive_Clear()
{
	memset(&Sensor_Msg.BLE_Data.L_X, 0, 12);
}
