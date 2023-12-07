#include "Camera.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "Transport.h"
Camera_ Camera;

void Camera_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	HAL_UART_Transmit(huart,pData,Size,100);
}

void Camera_QR_Code_Detection()
{
	Camera.task_sign = Task_QR_Code_Detection;
	HAL_UART_Transmit(&huart6, (uint8_t*)QR_Code, strlen(QR_Code), 100);
	HAL_UART_Receive_DMA(UART_Camera, Camera.Uart_Receive, 3);
}

void Camera_Object_Detection()
{
	Camera.task_sign = Task_Object_Detection;
	HAL_UART_Transmit(UART_Camera, (uint8_t*)Object, strlen(Object), 100);
	HAL_UART_Receive_DMA(UART_Camera, Camera.Uart_Receive, 3);
}

void Camera_Msg_Clear()
{
	HAL_UART_Transmit(UART_Camera, (uint8_t*)No_Msg, strlen(No_Msg), 100);
}

void Camera_Receive_IT()
{
	if(Camera.task_sign == Task_QR_Code_Detection)
	{
		for(uint8_t i=0; i<3; i++)
		{
			switch(Camera.Uart_Receive[i])
			{
				case '1':
					Transport.Camera_Object_Order[i] = Object_Red;
					break;
				case '2':
					Transport.Camera_Object_Order[i] = Object_Green;
					break;
				case '3':
					Transport.Camera_Object_Order[i] = Object_Blue;
					break;
				default:
					Transport.Camera_Object_Order[i] = Object_None;
			}
		}
		Camera.task_sign = Task_Null;
	}
	else if(Camera.task_sign == Task_Object_Detection)
	{
		if(memcmp(Camera.Uart_Receive,"RED",3) == 0)		Transport.Camera_Object_Detect[Transport.Camera_Object_Detecting_Num] = Object_Red;
		else if(memcmp(Camera.Uart_Receive,"GRE",3) == 0)	Transport.Camera_Object_Detect[Transport.Camera_Object_Detecting_Num] = Object_Green;
		else if(memcmp(Camera.Uart_Receive,"BLU",3) == 0)	Transport.Camera_Object_Detect[Transport.Camera_Object_Detecting_Num] = Object_Blue;
		Camera.task_sign = Task_Null;
	}
}