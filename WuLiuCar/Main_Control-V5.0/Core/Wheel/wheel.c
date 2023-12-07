#include "wheel.h"
#include "Master_Slave_Communication.h"
#include "PID.h"
#include "stdlib.h"
#include "lv_app_wheel.h" 
#include "lv_app_pid.h" 
#include "Motro_Control_Basic.h"
#include "tim.h"
Wheel_ Wheel;

void PID_Start()
{
	HAL_TIM_Base_Start_IT(&htim6);
}

void PID_Stop()
{
	HAL_TIM_Base_Stop_IT(&htim6);
}

void Wheel_Speed_Control(uint8_t Address, int16_t speed, uint8_t aacelerated)
{
	Motor_Speed_Control(UART_Wheel, Address, speed, aacelerated);
}

void Wheel_Group_Speed_Control(float v_x, float v_y, float w, uint8_t aacelerated)
{
	if(Wheel.Close_sign==true)	Wheel_Open();
	Wheel.SPD.FL = (v_y+v_x-w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.FR = (v_y-v_x+w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.BL = (v_y-v_x-w*(Car_a+Car_b))/Wheel_r;
	Wheel.SPD.BR = (v_y+v_x+w*(Car_a+Car_b))/Wheel_r;

	if(Wheel.SPD.FL != Wheel.SPD.FL_last)
	{
		Wheel_Speed_Control(Wheel_Addr_FL, Wheel.SPD.FL, aacelerated);
		Wheel.SPD.FL_last = Wheel.SPD.FL;
	}
	if(Wheel.SPD.BR != Wheel.SPD.BR_last)
	{
		Wheel_Speed_Control(Wheel_Addr_BR, Wheel.SPD.BR, aacelerated);
		Wheel.SPD.BR_last = Wheel.SPD.BR;
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
		Wheel_Speed_To_0(255);
		Motor_Close(UART_Wheel, Wheel_Addr_All);
		Wheel.Close_sign=true;
	}
}

void Wheel_Speed_To_0(uint8_t acc)
{
	Wheel_Group_Speed_Control(0, 0, 0, acc);
	Wheel_Closed_Loop_Spd_Set(0, 0, 0, acc);
}

void Wheel_MStep_Set(uint8_t num)  //设置细分步数
{	
	Motor_MStep_Set(UART_Wheel, Wheel_Addr_All, num);
}

void PID_Calculate_Control()
{
	if(Wheel.PID_Sign)
	{
		if(PID[0].debug_sign == true)		Wheel.PID_W.W = PID_Speed_Calculate(PID_Debug_Set_Value);
		else if(PID[1].debug_sign == true)	Wheel.PID_W.W = PID_Position_Calculate(PID_Debug_Set_Value);
		else 								Wheel.PID_W.W = PID_Position_Calculate(Wheel.PID_W.Yaw);
	}
	else 									Wheel.PID_W.W = Wheel.SPD.w;
	Wheel_Group_Speed_Control(Wheel.SPD.x, Wheel.SPD.y, Wheel.PID_W.W, Wheel.SPD.acc);
}

void Wheel_Closed_Loop_Init()
{
	Wheel.PID_W.Yaw = Sensor_Msg.IMU_Data.Yaw;
}

void Wheel_Closed_Loop_Spd_Set(float v_x, float v_y, float w, uint8_t aacelerated)
{
	Wheel.SPD.x = v_x;
	Wheel.SPD.y = v_y;
	Wheel.SPD.w = w;
	Wheel.PID_W.Yaw += w;
	Wheel.SPD.acc = aacelerated;
}

void Wheel_Init()
{
	Wheel.SPD.FL = 0;
	Wheel.SPD.FR = 0;
	Wheel.SPD.BL = 0;
	Wheel.SPD.BR = 0;
	Wheel.PID_W.Yaw = 0;
	Wheel.Close_sign = false;
	Wheel.PID_Sign = true;
	lv_obj_add_state(lv_obj_PID.sw_pid, LV_STATE_CHECKED);
	Wheel.debug_sign = false;
	Wheel_MStep_Set(0);
	Wheel_Close();

	PID[0].mode = PID_DELTA;
	PID[1].mode = PID_POSITION;
}

