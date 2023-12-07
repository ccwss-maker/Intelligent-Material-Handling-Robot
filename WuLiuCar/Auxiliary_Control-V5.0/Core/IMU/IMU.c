#include "IMU.h"
#include "string.h"
#include "math.h"
#include "Master_Slave_Communication.h"
uint8_t IMU_Receive_Data[Imu_Uart_Buffer];

uint8_t check(uint8_t n)
{
	uint8_t check=0X55;
	uint8_t i;
	for(i=n;i<n+9;i++)
	{
		check+=IMU_Receive_Data[i];
	}
	
	if(check==IMU_Receive_Data[i])
	{
		memcpy(Sensor_Msg.IMU_Data.data,&IMU_Receive_Data[n+1],6);
		return 1;
	}
	else return 0;
}

void IMU_Receive()
{
	for(uint8_t i=0;i<Imu_Uart_Buffer-8;i++)
	{
		switch(IMU_Receive_Data[i])
		{
			case 0x52:
				if(check(i))//角速度数据校验
				{
					i+=10;
					Sensor_Msg.IMU_Data.Wx=(float)Sensor_Msg.IMU_Data.data[0]/32768*2000/360*3.14f;
					Sensor_Msg.IMU_Data.Wy=(float)Sensor_Msg.IMU_Data.data[1]/32768*2000/360*3.14f;
					Sensor_Msg.IMU_Data.Wz=(float)Sensor_Msg.IMU_Data.data[2]/32768*2000/360*3.14f;
					Sensor_Msg.IMU_Data.W=pow(pow(Sensor_Msg.IMU_Data.Wx,2)+pow(Sensor_Msg.IMU_Data.Wy,2)+pow(Sensor_Msg.IMU_Data.Wz,2),0.5);
					Sensor_Msg.IMU_Data.chart_sign.W = true;
					continue;
				}
				break;
			case 0x53:
				if(check(i))//角度数据校验
				{
					i+=10;
					Sensor_Msg.IMU_Data.Roll=(float)Sensor_Msg.IMU_Data.data[0]/32768.0f*180;
					Sensor_Msg.IMU_Data.Pitch=(float)Sensor_Msg.IMU_Data.data[1]/32768.0f*180;
					Sensor_Msg.IMU_Data.Yaw=(float)Sensor_Msg.IMU_Data.data[2]/32768.0f*180;
					Sensor_Msg.IMU_Data.chart_sign.R = true;
					continue;
				}
		}
	}
}

void IMU_Z_Init()
{
	if(Sensor_Msg.IMU_Data.Yaw!=0)
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
