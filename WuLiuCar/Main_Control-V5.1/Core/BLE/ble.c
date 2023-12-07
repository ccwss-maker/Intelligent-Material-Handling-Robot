#include "ble.h"
#include "Master_Slave_Communication.h"
#include "Arm.h"
#include "PID.h"
#include "wheel.h"
#include "lv_app.h"
#include "lv_app_wheel.h"
#include "lv_app_pid.h"
#include "stdio.h"

BLE_Arm_ BLE_Arm;

void BLE_Arm_Control()
{
	bool sign = false;

	if(Sensor_Msg.BLE_Data.R_X != 0)	
	{
		BLE_Arm.x += (int)(Sensor_Msg.BLE_Data.R_X/100.0);
		if(BLE_Arm.x < Arm.Range.X_Min)	BLE_Arm.x = (int)Arm.Range.X_Min;
		else if(BLE_Arm.x > Arm.Range.X_Max)	BLE_Arm.x = (int)Arm.Range.X_Max;
		sign = true;
	}
	
	if(Sensor_Msg.BLE_Data.R_Y != 0)	
	{
		BLE_Arm.y += (int)(Sensor_Msg.BLE_Data.R_Y/100.0);
		if(BLE_Arm.y < Arm.Range.Y_Min)	BLE_Arm.y = (int)Arm.Range.Y_Min;
		else if(BLE_Arm.y > Arm.Range.Y_Max)	BLE_Arm.y = (int)Arm.Range.Y_Max;
		sign = true;
	}
	
	if((Sensor_Msg.BLE_Data.L_1 !=0 || Sensor_Msg.BLE_Data.R_1 != 0))	
	{
		BLE_Arm.z += 9*((int)Sensor_Msg.BLE_Data.R_1 - (int)Sensor_Msg.BLE_Data.L_1);
		if(BLE_Arm.z < Arm.Range.Z_Min)	BLE_Arm.z = (int)Arm.Range.Z_Min;
		else if(BLE_Arm.z > Arm.Range.Z_Max)	BLE_Arm.z = (int)Arm.Range.Z_Max;
		sign = true;
	}

	if(sign)
	{
		lv_slider_set_value(lv_obj_ARM.Motor_Control.slider_X.slider, BLE_Arm.x, LV_ANIM_ON);
		lv_slider_set_value(lv_obj_ARM.Motor_Control.slider_Y.slider, BLE_Arm.y, LV_ANIM_ON);
		lv_slider_set_value(lv_obj_ARM.Motor_Control.slider_Z.slider, BLE_Arm.z, LV_ANIM_ON);
		char buf[20]={0};
		sprintf(buf, "%d", BLE_Arm.x);
		lv_label_set_text(lv_obj_ARM.Motor_Control.slider_X.slider_label, buf);
		sprintf(buf, "%d", BLE_Arm.y);
		lv_label_set_text(lv_obj_ARM.Motor_Control.slider_Y.slider_label, buf);
		sprintf(buf, "%d", BLE_Arm.z);
		lv_label_set_text(lv_obj_ARM.Motor_Control.slider_Z.slider_label, buf);
		lv_Arm_Coord_Control();
	}
	
	if(Sensor_Msg.BLE_Data.A==1&&Sensor_Msg.BLE_Data.B==0)
	{
		Claw_Open();
	}
	else if(Sensor_Msg.BLE_Data.A==0&&Sensor_Msg.BLE_Data.B==1)
	{
		Claw_Close();
	}
	
	if(Sensor_Msg.BLE_Data.R_3==1)
	{
		lv_Arm_Coord_Control_Reset();
	}
}

void BLE_wheel_control()
{
	if(Sensor_Msg.BLE_Data.L_X>-100&&Sensor_Msg.BLE_Data.L_X<100)	Sensor_Msg.BLE_Data.L_X=0;
	if(Sensor_Msg.BLE_Data.L_Y>-100&&Sensor_Msg.BLE_Data.L_Y<100)	Sensor_Msg.BLE_Data.L_Y=0;

	if(Sensor_Msg.BLE_Data.L_3==1)	
	{
		Wheel_Close();
	}
	else if(Sensor_Msg.BLE_Data.L_X!=0||Sensor_Msg.BLE_Data.L_Y!=0||Sensor_Msg.BLE_Data.L_2>0||Sensor_Msg.BLE_Data.R_2>0)
	{
		Wheel_Group_Closed_Loop_Control(Sensor_Msg.BLE_Data.L_X/1000.0, Sensor_Msg.BLE_Data.L_Y/1000.0, (Sensor_Msg.BLE_Data.L_2-Sensor_Msg.BLE_Data.R_2)/200.0,255);
	}
	else if(Sensor_Msg.BLE_Data.left==1)
	{
		Wheel_Group_Closed_Loop_Control(-1, 0, 0,255);
	}
	else if(Sensor_Msg.BLE_Data.right==1)
	{
		Wheel_Group_Closed_Loop_Control(1, 0, 0,255);
	}
	else if(Sensor_Msg.BLE_Data.up==1)
	{
		Wheel_Group_Closed_Loop_Control(0, 1, 0,255);
	}
	else if(Sensor_Msg.BLE_Data.down==1)
	{
		Wheel_Group_Closed_Loop_Control(0, -1, 0,255);
	}
	else
	{
		Wheel_Speed_To_0();
	}
}

void BLE_control()
{
	if(PID[0].debug_sign || PID[1].debug_sign)
	{
		if(PID[0].BLE_debug_sign == true)	PID_Debug_Set_BLE(0);
		if(PID[1].BLE_debug_sign == true)	PID_Debug_Set_BLE(1);
		Wheel_Group_Speed_Control(0, 0, PID_Debug_Calculate_Value, 255);
	}
	else if(Wheel.debug_sign==false)
	{
		BLE_wheel_control();
	}

	if(BLE_Arm.Control_Sign)
	{
		BLE_Arm_Control();
		BLE_Arm.Control_Sign = false;
	}
}

void BLE_Arm_Init()
{
	BLE_Arm.x = Arm_Coord_X_0;
	BLE_Arm.y = Arm_Coord_Y_0;
	BLE_Arm.z = Arm_Coord_Z_0;
}