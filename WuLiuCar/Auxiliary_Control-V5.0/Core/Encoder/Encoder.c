#include "Encoder.h"
#include "spi.h"
#include "Master_Slave_Communication.h"

uint8_t CRC8(uint8_t* pBuffer, uint16_t bufSize)
{
	uint8_t crc = 0;
	while( bufSize-- != 0 )
	{
		for ( uint16_t i = 0x80; i != 0; i /= 2 )
		{
			if ( (crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= 0x07; // 多项式：X8 + X2 + X + 1
			}
			else
			{
				crc *= 2;
			}
			if ( (*pBuffer & i) != 0 )
			{
				crc ^= 0x07;
			}
		}
		pBuffer++;
	}
	return crc;
}

void CS_H(uint8_t x)
{
	switch(x)
	{
		case 1:
			CS1_H;
			break;
		case 2:
			CS2_H;
			break;
		case 3:
			CS3_H;
			break;
	}
}

void CS_L(uint8_t x)
{
	switch(x)
	{
		case 1:
			CS1_L;
			break;
		case 2:
			CS2_L;
			break;
		case 3:
			CS3_L;
			break;
	}
}

void Encoder(uint8_t i)
{
	uint8_t Encoder_Transmit[4][3] = {{0x30, 0x03, 0x00}, {0x30, 0x04, 0x00}, {0x30, 0x05, 0x00}, {0x30, 0x06, 0x00}};
	uint8_t Encoder_Receive_[4][3] = {0};
	uint8_t Encoder_Receive[4];
	uint32_t Sample_Data;
	for(uint8_t j=0; j<4; j++)
	{
		CS_L(i);
		HAL_SPI_TransmitReceive(&hspi1, Encoder_Transmit[j], Encoder_Receive_[j], 3, HAL_MAX_DELAY);
		CS_H(i);
		Encoder_Receive[j]=Encoder_Receive_[j][2];
	}
	if(CRC8(Encoder_Receive, 3) == Encoder_Receive[3])
	{
		Sample_Data = ((uint32_t)Encoder_Receive[0] << 13) | ((uint32_t)Encoder_Receive[1] << 5) | ((uint32_t)Encoder_Receive[2] >> 3);
		Sensor_Msg.Encoder_Data[i-1].angle = (float)Sample_Data * 360.f / 2097152.f;
		Sensor_Msg.Encoder_Data[i-1].Over_Speed_Flag = (bool)(Encoder_Receive[2]>>0 & 0x01);
		Sensor_Msg.Encoder_Data[i-1].Weak_Magnetic_Flag = (bool)(Encoder_Receive[2]>>1 & 0x01);
		Sensor_Msg.Encoder_Data[i-1].Low_Voltage_Flag = (bool)(Encoder_Receive[2]>>2 & 0x01);
	}
}

void Encoder_Detection()
{
	Encoder(1);
	Encoder(2);
	Encoder(3);
}
