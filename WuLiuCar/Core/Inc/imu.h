#ifndef imu_h
#define imu_h
#include <usart.h>
#define imu_uart_buffer 33
#define imu_acc_start 0
#define imu_ang_vel_start 11
#define imu_angle_start 22
typedef struct{
	volatile uint8_t rx_len;  
	volatile uint8_t recv_end_flag; 
	uint8_t receive_imu[imu_uart_buffer];
}imu_uart_;

typedef struct{
	short data[3];
	
	float Ax;
	float Ay;
	float Az;
	
	float Wx;
	float Wy;
	float Wz;
	volatile 	float W;
	
	float Roll;
	float Pitch;
	float Yaw;
}message_imu_;

void IMU_Receive(void);
void IMU_Init(void);
void IMU_Z_Init(void);
#endif
