#ifndef Master_Slave_Communication_H
#define Master_Slave_Communication_H
#include "main.h"

#define Tracking_Front 	0
#define Tracking_Back 	1
#define Tracking_Left 	2
#define Tracking_Right 	3
//////////////////////////IMU///////////////////////////
typedef struct{
	bool A;
	bool W;
	bool R;
}chart_sign_;

typedef struct{
	short data[3];
	
	float Ax;
	float Ay;
	float Az;
	
	float Wx;
	float Wy;
	float Wz;
	float W;
	
	float Roll;
	float Pitch;
	float Yaw;
	
	chart_sign_ chart_sign;
}IMU_Data_;
//////////////////////////BLE///////////////////////////
typedef struct{
	bool Connected_Sign;
	uint8_t L_3;
	uint8_t L_1;
	uint8_t R_3;
	uint8_t R_1;
	uint8_t up;
	uint8_t down;
	uint8_t left;
	uint8_t right;
	uint8_t A;
	uint8_t B;
	uint8_t X;
	uint8_t Y;

	int16_t digital_data;/*L_3;L_1;R_3;R_1;up;down;left;right;A;B;X;Y;*/
	int16_t L_X;
	int16_t L_Y;
	int16_t L_2;
	int16_t R_X;
	int16_t R_Y;
	int16_t R_2;
	int16_t check;/*如果值最终为0，说明没有按键按*/
}BLE_Data_;
//////////////////////////Encoder///////////////////////////
typedef struct{
	float angle;
	bool Over_Speed_Flag;
	bool Weak_Magnetic_Flag;
	bool Low_Voltage_Flag;
}Encoder_Data_;

//////////////////////////Sensor_Msg///////////////////////////
typedef struct{
	IMU_Data_ IMU_Data;
	BLE_Data_ BLE_Data;
	Encoder_Data_ Encoder_Data[3];
	uint8_t Tracking_Data[4];
	float Battery_Voltage;
	uint32_t check;
}Sensor_Msg_;

void Master_Slave_Communication_Init();
void Master_Slave_Communication_Receive();

extern Sensor_Msg_ Sensor_Msg;

#endif
