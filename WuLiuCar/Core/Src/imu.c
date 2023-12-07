#include <imu.h>
#include <string.h>
#include <math.h>
message_imu_ message_imu;

imu_uart_ imu_uart={0,0,{0}};
uint8_t check(uint8_t n)
{
	uint8_t check=0;
	uint8_t i;
	for(i=n;i<n+10;i++)
	{
		check+=imu_uart.receive_imu[i];
	}
	
	if(check==imu_uart.receive_imu[i])
	{
		memcpy(message_imu.data,&imu_uart.receive_imu[n+2],6);
		return 1;
	}
	else return 0;
}

void IMU_Receive()
{
		if(check(imu_acc_start))	//加速度数据校验
		{
			message_imu.Ax=(float)message_imu.data[0]/32768*16*9.8;
			message_imu.Ay=(float)message_imu.data[1]/32768*16*9.8;
			message_imu.Az=(float)message_imu.data[2]/32768*16*9.8;
		}
		if(check(imu_ang_vel_start))	//角速度数据校验
		{
			message_imu.Wx=(float)message_imu.data[0]/32768*2000;
			message_imu.Wy=(float)message_imu.data[1]/32768*2000;
			message_imu.Wz=(float)message_imu.data[2]/32768*2000;
			message_imu.W=pow(pow(message_imu.Wx,2)+pow(message_imu.Wy,2)+pow(message_imu.Wz,2),0.5);
		}
		if(check(imu_angle_start))	//角度数据校验
		{
			message_imu.Roll=(float)message_imu.data[0]/32768.0*180;
			message_imu.Pitch=(float)message_imu.data[1]/32768.0*180;
			message_imu.Yaw=(float)message_imu.data[2]/32768.0*180;
		}
}

void IMU_Z_Init()
{
	if(message_imu.Yaw!=0)
	{
		uint8_t data[3]={0xFF,0XAA};
		data[2]=0x52;	//Z轴角度归零
		HAL_UART_Transmit(&huart2,data,3,HAL_MAX_DELAY);
		HAL_Delay(1);
	}
}
void IMU_Init()
{
	uint8_t data[3]={0xFF,0XAA};
	data[2]=0x65;	//水平安装
	HAL_UART_Transmit(&huart2,data,3,HAL_MAX_DELAY);
	HAL_Delay(10);
	data[2]=0x52;	//Z轴角度归零
	HAL_UART_Transmit(&huart2,data,3,HAL_MAX_DELAY);
	HAL_Delay(10);
	data[2]=0x67;	//加计校准
	HAL_UART_Transmit(&huart2,data,3,HAL_MAX_DELAY);
	HAL_Delay(10);
}