#include "Motro_Control_Wheel.h"
#include "Motro_Control_Basic.h"
Motro_Control_Wheel_ Motro_Control_Wheel;

void Motro_Transmit_Verify_Wheel(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint8_t* pVerify)
{
	if(Motro_Control_Wheel.feedback_sign == true)
	{
		uint8_t receive[3];
		uint8_t times = 0;
		do{
			HAL_UART_Transmit(huart,pData,Size,20);
			if(HAL_UART_Receive(huart,receive,3,20)==HAL_OK && memcmp(receive,pVerify,3) == 0)
			{
				break;
			}
			times++;
		}while(times < Motro_Control_Wheel.feedback_error);
	}
	else
	{
		HAL_UART_Transmit(huart,pData,Size,20);
		HAL_Delay(1);
	}
}