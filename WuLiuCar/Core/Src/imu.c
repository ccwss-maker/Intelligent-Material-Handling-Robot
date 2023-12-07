#include <imu.h>
#include <string.h>
#include <math.h>
message_imu_ message_imu;

imu_uart_ imu_uart;
uint8_t check(uint8_t n)
{
	uint8_t check=0X55;
	uint8_t i;
	for(i=n;i<n+9;i++)
	{
		check+=imu_uart.receive[i];
	}
	
	if(check==imu_uart.receive[i])
	{
		memcpy(message_imu.data,&imu_uart.receive[n+1],6);
		return 1;
	}
	else return 0;
}

void IMU_Receive()
{
	for(uint8_t i=0;i<imu_uart_buffer-8;i++)
	{
		switch(imu_uart.receive[i])
		{
			case 0x51: 
				if(check(i))//加速度数据校验
				{
					i+=10;
					message_imu.Ax=(float)message_imu.data[0]/32768*16*9.8f;
					message_imu.Ay=(float)message_imu.data[1]/32768*16*9.8f;
					message_imu.Az=(float)message_imu.data[2]/32768*16*9.8f;
					message_imu.chart_sign.A = true;
					continue;
				}
				break;
			case 0x52:
				if(check(i))//角速度数据校验
				{
					i+=10;
					message_imu.Wx=(float)message_imu.data[0]/32768*2000/360*3.14f;
					message_imu.Wy=(float)message_imu.data[1]/32768*2000/360*3.14f;
					message_imu.Wz=(float)message_imu.data[2]/32768*2000/360*3.14f;
					message_imu.W=pow(pow(message_imu.Wx,2)+pow(message_imu.Wy,2)+pow(message_imu.Wz,2),0.5);
					message_imu.chart_sign.W = true;
					continue;
				}
				break;
			case 0x53:
				if(check(i))//角度数据校验
				{
					i+=10;
					message_imu.Roll=(float)message_imu.data[0]/32768.0f*180;
					message_imu.Pitch=(float)message_imu.data[1]/32768.0f*180;
					message_imu.Yaw=(float)message_imu.data[2]/32768.0f*180;
					message_imu.chart_sign.R = true;
					continue;
				}
		}
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

void IMU_Baud_115200()
{
	uint8_t data[]={0XFF,0XAA,0X63};
	HAL_UART_Transmit(&huart2,data,3,HAL_MAX_DELAY);
}

void IMU_Baud_9600()
{
	uint8_t data[]={0XFF,0XAA,0X64};
	HAL_UART_Transmit(&huart2,data,3,HAL_MAX_DELAY);
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