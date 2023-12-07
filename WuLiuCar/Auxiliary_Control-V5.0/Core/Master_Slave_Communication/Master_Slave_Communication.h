#ifndef Master_Slave_Communication_H
#define Master_Slave_Communication_H
#include "main.h"
#include "IMU.h"
#include "BLE.h"
#include "Encoder.h"
#include "Tracking.h"
#include "Battery.h"

typedef struct{
	IMU_Data_ IMU_Data;
	BLE_Data_ BLE_Data;
	Encoder_Data_ Encoder_Data[3];
	uint8_t Tracking_Data[4];
	float Battery_Voltage;
	uint32_t check;
}Sensor_Msg_;

void Master_Slave_Communication_Init();
void Master_Slave_Communication();
void Master_Slave_Communication_Receive();

extern Sensor_Msg_ Sensor_Msg;

#endif
