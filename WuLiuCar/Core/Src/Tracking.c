#include "Tracking.h"

Tracking_ Tracking={{0},0x01};

void Tracking_Transmit()
{
	if(Tracking.Slaver_Address != 0x04)
	{
		Tracking.Slaver_Address++;
	}
	else
	{
		Tracking.Slaver_Address = 0x01;
	}
	uint8_t data=0xFF;
	HAL_I2C_Master_Transmit_DMA(&hi2c3,Tracking.Slaver_Address<<1,&data,1);
}

void Tracking_Receive()
{
	HAL_I2C_Master_Receive_DMA(&hi2c3,Tracking.Slaver_Address<<1,&Tracking.data[Tracking.Slaver_Address-1],1);
}

void Tracking_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	if (HAL_I2C_GetError(hi2c) != HAL_I2C_ERROR_AF) 
	{
		hi2c->State = HAL_I2C_STATE_RESET;
	}
}


