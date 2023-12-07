#include "Master_Slave_Communication.h"
#include "spi.h"
#include "crc.h"
#include "stdio.h"
#include "string.h"
#include "Arm.h"
Sensor_Msg_ Sensor_Msg_Pre;
Sensor_Msg_ Sensor_Msg;

void Master_Slave_Communication_Init()
{
	HAL_SPI_Receive_DMA(&hspi2, (uint8_t*)&Sensor_Msg_Pre, sizeof(Sensor_Msg_Pre)/2);
}

void Master_Slave_Communication_Receive()
{
	uint32_t check = HAL_CRC_Calculate(&hcrc, (uint32_t*)&Sensor_Msg_Pre, sizeof(Sensor_Msg_Pre)/4-1);
	if(check == Sensor_Msg_Pre.check)
	{
		memcpy(&Sensor_Msg, &Sensor_Msg_Pre, sizeof(Sensor_Msg_Pre));
	}
}
