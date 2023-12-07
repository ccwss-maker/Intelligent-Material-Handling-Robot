#ifndef imu_h
#define imu_h
#include <usart.h>
#define imu_uart_buffer 33
typedef struct{
	uint8_t receive[imu_uart_buffer];
}imu_uart_;

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
}message_imu_;

void IMU_Receive(void);
void IMU_Init(void);
void IMU_Z_Init(void);

extern message_imu_ message_imu;
extern imu_uart_ imu_uart;
#endif
