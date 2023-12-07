#include "Motro_Control_Arm.h"
#include "Motro_Control_Basic.h"
#include "Arm.h"
#include "lv_app_motro_control.h"
#include "Master_Slave_Communication.h"
#include "string.h" //memcpy
Motro_Control_Arm_ Motro_Control_Arm;

void Motro_Transmit_Verify_Arm(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint8_t* pVerify)
{
	if(Motro_Control_Arm.feedback_sign == true)
	{
		lv_obj_Motro_Control.Control.times++;
		uint8_t receive[3];
		uint8_t times = 0;
		do{
			HAL_UART_Transmit(huart,pData,Size,20);
			if(HAL_UART_Receive(huart,receive,3,20)==HAL_OK)
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
		}while(times < Motro_Control_Arm.feedback_error);
	}
	else
	{
		HAL_UART_Transmit(huart,pData,Size,20);
		HAL_Delay(10);
	}
}

/*绝对位置控制*/
void Motor_Absolute_Control(uint8_t Address, uint16_t speed, uint8_t accelerated, float angle)
{
	Motor_Relative_Position_Stop(UART_Arm, Address);
	float Relative_Angle, Reduction_Ratio;
	int8_t direction;
	switch(Address)
	{
		case Arm_Rear_Address:	
			Relative_Angle = Rear_Angle - angle;
			Reduction_Ratio = Arm_Rear_Reduction_Ratio;
			direction = Counterclockwise;
			break;
		case Arm_Front_Address:	
			Relative_Angle = Front_Angle - angle;
			Reduction_Ratio = Arm_Front_Reduction_Ratio;
			direction = Counterclockwise;
			break;
		case Arm_Pedestal_Address:	
			Relative_Angle = Pedestal_Angle - angle;
			Reduction_Ratio = Arm_Pedestal_Reduction_Ratio;
			direction = Clockwise;
			break;
	}
    Motor_Relative_Position_Control(UART_Arm, Address, direction, speed, accelerated, Relative_Angle, Arm_Subdivision_Step, Reduction_Ratio);
}