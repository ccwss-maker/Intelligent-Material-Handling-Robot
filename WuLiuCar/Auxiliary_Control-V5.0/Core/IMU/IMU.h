#ifndef IMU_H
#define IMU_H
#include <usart.h>
#define Imu_Uart_Buffer 33

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

void IMU_Receive(void);
void IMU_Init(void);
void IMU_Z_Init(void);

extern uint8_t IMU_Receive_Data[Imu_Uart_Buffer];
#endif
