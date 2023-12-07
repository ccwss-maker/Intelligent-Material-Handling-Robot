#include "Tracking.h"
#include "i2c.h"
#include "Master_Slave_Communication.h"

uint8_t Tracking_Slaver_Address = 0x01;

uint8_t reverse_bits(uint8_t num)
{
	uint8_t res = 0;
    for (int i=0; i<8; i++)
    {
        res <<= 1;   // 左移一位，为下一位做准备
        res |= num & 1;   // 将原数的一位加入新数中
        num >>= 1;   // 右移一位，为下一位做准备
    }
    return res;
}


void Tracking_Detection()
{

	if(Tracking_Slaver_Address != 0x04)		Tracking_Slaver_Address++;
	else									Tracking_Slaver_Address = 0x01;
	uint8_t data=0xFF;
	HAL_I2C_Master_Transmit(&hi2c1,Tracking_Slaver_Address<<1,&data,1,100);
	HAL_I2C_Master_Receive(&hi2c1,Tracking_Slaver_Address<<1,&Sensor_Msg.Tracking_Data[Tracking_Slaver_Address-1],1,100);

	if(Tracking_Slaver_Address == 0x03 || Tracking_Slaver_Address == 0x02)
	{
		Sensor_Msg.Tracking_Data[Tracking_Slaver_Address-1] = reverse_bits(Sensor_Msg.Tracking_Data[Tracking_Slaver_Address-1]);
	}
}

void Tracking_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	if (HAL_I2C_GetError(hi2c) != HAL_I2C_ERROR_AF)
	{
		hi2c->State = HAL_I2C_STATE_RESET;
	}
}
