#include "wheel.h"
#include "imu.h"
#include "PID.h"
#include "stdlib.h"
#include "lv_app_wheel.h" 
#include "Motro_Control.h"

Wheel_ Wheel;

void Wheel_Group_Speed_Control(float v_x, float v_y, float w, uint8_t aacelerated)
{
	Wheel.SPD.FL = (v_y+v_x-w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.FR = (v_y-v_x+w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.BL = (v_y-v_x-w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.BR = (v_y+v_x+w*(Car_a+Car_b))/Wheel_r;
	if(Wheel.SPD.FL != Wheel.SPD.FL_last)
	{
		Motor_Speed_Control(1,Wheel.SPD.FL,aacelerated);
		Wheel.SPD.FL_last = Wheel.SPD.FL;
	}
	if(Wheel.SPD.FR != Wheel.SPD.FR_last)
	{
		Motor_Speed_Control(2,Wheel.SPD.FR,aacelerated);
		Wheel.SPD.FR_last = Wheel.SPD.FR;
	}
	if(Wheel.SPD.BL != Wheel.SPD.BL_last)
	{
		Motor_Speed_Control(3,Wheel.SPD.BL,aacelerated);
		Wheel.SPD.BL_last = Wheel.SPD.BL;
	}
	if(Wheel.SPD.BR != Wheel.SPD.BR_last)
	{
		Motor_Speed_Control(4,Wheel.SPD.BR,aacelerated);
		Wheel.SPD.BR_last = Wheel.SPD.BR;
	}
}

float PID_Speed(float Speed)
{
	Speed = PID_calc(&PID[0],message_imu.Wz,Speed);//速度环
	return Speed;
}

float PID_Position(float Angle)
{
	float Speed = PID_calc(&PID[1],message_imu.Yaw, Angle);
	Speed = PID_Speed(Speed);
	return Speed;
}

void PID_Debug_Speed(float Speed)
{
	Speed = PID_Speed(Speed);
	Wheel_Group_Speed_Control(0, 0, Speed, 255);
}

void PID_Debug_Position(float Angle)
{
	float Speed = PID_Position(Angle);
	Wheel_Group_Speed_Control(0, 0, Speed, 255);
}

void Wheel_Open()
{
	for(uint8_t i=1; i<5; i++)
	{
		Motor_Open(i);
	}
	Wheel.Close_sign=false;
}

void Wheel_Close()
{
	Wheel_Group_Closed_Loop_Control(0, 0, 0, 255);
	// do{
	// 	lv_task_handler();
	// }while (message_imu.W>0.1);
	for(uint8_t i=1; i<5; i++)
	{
		Motor_Close(i);
	}
	Wheel.Close_sign=true;
}

void Wheel_MStep_Set(uint8_t num)  //设置细分步数
{	
	for(uint8_t i=1; i<5; i++)
	{
		Motor_MStep_Set(i, num);
	}
}

void Wheel_Group_Closed_Loop_Control(float v_x, float v_y, float w, uint8_t aacelerated)
{
	static float Yaw = 0;
	if(Wheel.Close_sign == true)
	{
		Wheel_Open();
	}
	if(Wheel.PID_Sign == true)
	{
		if(w==0)
		{
			w = PID_Position(Yaw);
		}
		else
		{
	 		Yaw = message_imu.Yaw;
		}
	}
	Wheel_Group_Speed_Control(v_x, v_y, w, aacelerated);
}

void Wheel_Init()
{
	Wheel.SPD.FL=0;
	Wheel.SPD.FR=0;
	Wheel.SPD.BL=0;
	Wheel.SPD.BR=0;
	Wheel.Close_sign=false;
	Wheel.PID_Sign=true;
	Wheel.debug_sign=false;
	// Wheel_Close();
	// Wheel_MStep_Set(0);

	PID[0].mode = PID_DELTA;
	PID[1].mode = PID_POSITION;
		
	// PID_init(&PID[0], PID_DELTA, 1.9, 0.2, 4.5, 11, 11, 11, 0.1);    //速度
	// PID_init(&PID[1], PID_POSITION, 0.025,0.0005,0.195,0.35,3,3,1); //位置

}

