#include "Motro_Control_Wheel.h"
#include "Motro_Control_Basic.h"
#include "Motro_Control_Queul.h"
#include "lv_app_motro_control.h"
#include "tim.h"
Motro_Control_Wheel_ Motro_Control_Wheel;

void Motro_Transmit_Verify_Wheel(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint8_t* pVerify)
{
	if(Motro_Control_Wheel.feedback_sign == true)
	{
		lv_obj_Motro_Control.Control.times++;
		uint8_t receive[3];
		uint8_t times = 0;
		do{
			HAL_UART_Transmit(huart,pData,Size,5);
			if(HAL_UART_Receive(huart,receive,3,10)==HAL_OK)
			{
				lv_obj_Motro_Control.Receive_Successful.times++;
				if(memcmp(receive,pVerify,3) == 0)
				{
					lv_obj_Motro_Control.Verify_Successful.times++;
					if(times == 0)	lv_obj_Motro_Control.Control_Success_Only_Once.times++;
					break;
				}
				else
				{
					lv_obj_Motro_Control.Verify_Failure.times++;
				}
			}
			else	
			{
				lv_obj_Motro_Control.Receive_Failure.times++;
				lv_obj_Motro_Control.Control_Error_Only_Once.times++;
			}
			times++;
		}while(times < 2);
	}
	else
	{
		HAL_UART_Transmit(huart,pData,Size,20);
		HAL_Delay(1);
	}
}