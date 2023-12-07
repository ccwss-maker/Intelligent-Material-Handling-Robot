#include "wheel.h"
#include "Master_Slave_Communication.h"
#include "PID.h"
#include "stdlib.h"
#include "lv_app_wheel.h" 
#include "Motro_Control_Basic.h"

Wheel_ Wheel;

void Wheel_Speed_Control(uint8_t Address, int16_t speed, uint8_t aacelerated)
{
	Motor_Speed_Control(UART_Wheel, Address, speed, aacelerated);
}

void Wheel_Group_Speed_Control(float v_x, float v_y, float w, uint8_t aacelerated)
{
	Wheel.SPD.FL = (v_y+v_x-w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.FR = (v_y-v_x+w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.BL = (v_y-v_x-w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.BR = (v_y+v_x+w*(Car_a+Car_b))/Wheel_r;
	if(Wheel.SPD.FL != Wheel.SPD.FL_last)
	{
		Wheel_Speed_Control(Wheel_Addr_FL, Wheel.SPD.FL, aacelerated);
		Wheel.SPD.FL_last = Wheel.SPD.FL;
	}
	if(Wheel.SPD.FR != Wheel.SPD.FR_last)
	{
		Wheel_Speed_Control(Wheel_Addr_FR, Wheel.SPD.FR, aacelerated);
		Wheel.SPD.FR_last = Wheel.SPD.FR;
	}
	if(Wheel.SPD.BL != Wheel.SPD.BL_last)
	{
		Wheel_Speed_Control(Wheel_Addr_BL, Wheel.SPD.BL, aacelerated);
		Wheel.SPD.BL_last = Wheel.SPD.BL;
	}
	if(Wheel.SPD.BR != Wheel.SPD.BR_last)
	{
		Wheel_Speed_Control(Wheel_Addr_BR, Wheel.SPD.BR, aacelerated);
		Wheel.SPD.BR_last = Wheel.SPD.BR;
	}
}

float PID_Speed_Calculate(float Speed)
{
	Speed = PID_calc(&PID[0],Sensor_Msg.IMU_Data.Wz,Speed);//速度环
	return Speed;
}

float PID_Position_Calculate(float Angle)
{
	float Speed = PID_calc(&PID[1],Sensor_Msg.IMU_Data.Yaw, Angle);
	Speed = PID_Speed_Calculate(Speed);
	return Speed;
}

void Wheel_Open()
{
	Motor_Open(UART_Wheel, Wheel_Addr_All);
	Wheel.Close_sign=false;
}

void Wheel_Close()
{
	if(!Wheel.Close_sign)
	{
		Wheel_Group_Closed_Loop_Control(0, 0, 0, 255);
		// do{
		// 	lv_task_handler();
		// }while (message.imu.W>0.1);
		Motor_Close(UART_Wheel, Wheel_Addr_All);
		Wheel.Close_sign=true;
	}
}

void Wheel_Speed_To_0()
{
	Wheel_Group_Closed_Loop_Control(0,0,0,255);
}

void Wheel_MStep_Set(uint8_t num)  //设置细分步数
{	
	Motor_MStep_Set(UART_Wheel, Wheel_Addr_All, num);
}

void PID_Calculate()
{
	if(Wheel.PID_Sign)
	{
		if(PID[0].debug_sign == true)		PID_Debug_Calculate_Value = PID_Speed_Calculate(PID_Debug_Set_Value);
		else if(PID[1].debug_sign == true)	PID_Debug_Calculate_Value = PID_Position_Calculate(PID_Debug_Set_Value);
		else Wheel.PID_W.W = PID_Position_Calculate(Wheel.PID_W.Yaw);
	}
}

void Wheel_Group_Closed_Loop_Control(float v_x, float v_y, float w, uint8_t aacelerated)
{
	if(Wheel.Close_sign==true)	Wheel_Open();
	if(Wheel.PID_Sign)
	{
		if(w!=0)	Wheel.PID_W.Yaw += w;
	}
	else	Wheel.PID_W.W = w;
	Wheel_Group_Speed_Control(v_x, v_y, Wheel.PID_W.W, aacelerated);
}

void Wheel_Init()
{
	Wheel.SPD.FL = 0;
	Wheel.SPD.FR = 0;
	Wheel.SPD.BL = 0;
	Wheel.SPD.BR = 0;
	Wheel.PID_W.Yaw = 0;
	Wheel.Close_sign = false;
	Wheel.PID_Sign = false;
	Wheel.debug_sign = false;
	// Wheel_Close();
	// Wheel_MStep_Set(0);

	PID[0].mode = PID_DELTA;
	PID[1].mode = PID_POSITION;
}

