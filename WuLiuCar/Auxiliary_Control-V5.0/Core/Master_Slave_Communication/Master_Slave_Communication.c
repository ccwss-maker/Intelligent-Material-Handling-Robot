#include "Master_Slave_Communication.h"
#include "spi.h"
#include "crc.h"
#include "stdio.h"
#include "string.h"

uint8_t Master_Slave_Communication_Receive_Data;
Sensor_Msg_ Sensor_Msg;

void Master_Slave_Communication_Init()
{
	Sensor_Msg.BLE_Data.Connected_Sign = false;
}

void Master_Slave_Communication()
{
	Sensor_Msg.check = HAL_CRC_Calculate(&hcrc, (uint32_t*)&Sensor_Msg, sizeof(Sensor_Msg)/4-1);
	HAL_SPI_Transmit(&hspi2, (uint8_t*)&Sensor_Msg, sizeof(Sensor_Msg)/2, HAL_MAX_DELAY);
}
