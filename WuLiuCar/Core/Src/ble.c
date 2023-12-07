#include "ble.h"
#include "tim.h"
#include "Arm.h"
#include "PID.h"
#include "wheel.h"
#include "lv_app.h"
#include "lv_app_wheel.h"
#include "lv_app_pid.h"
#include "stdio.h"
ble_data_ ble_data;
ble_uart_ ble_uart;
BLE_Arm_ BLE_Arm;

void BLE_Receive()
{
    if(ble_uart.receive[0]!=0x66)	return;
	
	ble_data_ ble_data_check;
	memcpy(&ble_data_check.head,ble_uart.receive,18);
	
	int16_t check=0;
	for(int i=0;i<7;i++)
	{
		check^=*(&ble_data_check.digital_data+i);
	}
	
	if(check!=ble_data_check.check)	return;
    uint8_t msg=0x88;
    HAL_UART_Transmit_DMA(&huart6,&msg,1);

	for(int i=0;i<12;i++)
	{
		if((ble_data_check.digital_data>>i&(int16_t)1)==1)	*(&(ble_data_check.L_3)+i)=1;
		else												*(&(ble_data_check.L_3)+i)=0;
	}
	memcpy(&ble_data,&ble_data_check,sizeof(ble_data_check));
    
	if(ble_data.L_2>2000)
	{
		ble_data.L_2-=2000;
	}
	else
	{
		ble_data.L_2=0;
	}

	if(ble_data.R_2>2000)
	{
		ble_data.R_2-=2000;
	}
	else
	{
		ble_data.R_2=0;
	}
	ble_data.break_sign=true;
    
    char data[10]={0};
    sprintf(data,"0x%02x",ble_data.digital_data);
	lv_label_set_text(lv_obj_BLE.lable.digital_data,data);
    sprintf(data,"%d",ble_data.L_X);
	lv_label_set_text(lv_obj_BLE.lable.L_X,data);
    sprintf(data,"%d",ble_data.L_Y);
	lv_label_set_text(lv_obj_BLE.lable.L_Y,data);
    sprintf(data,"%d",ble_data.L_2);
	lv_label_set_text(lv_obj_BLE.lable.L_2,data);
    sprintf(data,"%d",ble_data.R_X);
	lv_label_set_text(lv_obj_BLE.lable.R_X,data);
    sprintf(data,"%d",ble_data.R_Y);
	lv_label_set_text(lv_obj_BLE.lable.R_Y,data);
    sprintf(data,"%d",ble_data.R_2);
	lv_label_set_text(lv_obj_BLE.lable.R_2,data);
}

void BLE_Arm_Control()
{
	if(BLE_Arm.Control_Sign)
	{
		bool sign = false;

		if(ble_data.R_X != 0)	
		{
			BLE_Arm.x += (int)(ble_data.R_X/300.0);
			if(BLE_Arm.x < Arm.Range.X_Min)	BLE_Arm.x = (int)Arm.Range.X_Min;
			else if(BLE_Arm.x > Arm.Range.X_Max)	BLE_Arm.x = (int)Arm.Range.X_Max;
			sign = true;
		}
		
		if(ble_data.R_Y != 0)	
		{
			BLE_Arm.y += (int)(ble_data.R_Y/300.0);
			if(BLE_Arm.y < Arm.Range.Y_Min)	BLE_Arm.y = (int)Arm.Range.Y_Min;
			else if(BLE_Arm.y > Arm.Range.Y_Max)	BLE_Arm.y = (int)Arm.Range.Y_Max;
			sign = true;
		}
		
		if((ble_data.L_1 !=0 || ble_data.R_1 != 0))	
		{
			BLE_Arm.z += 3*((int)ble_data.R_1 - (int)ble_data.L_1);
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
		
		if(ble_data.A==1&&ble_data.B==0)
		{
			Claw_Open();
		}
		else if(ble_data.A==0&&ble_data.B==1)
		{
			Claw_Close();
		}
		
		if(ble_data.R_3==1)
		{
			lv_Arm_Coord_Control_Reset();
		}
		BLE_Arm.Control_Sign = false;
	}
}

void BLE_PID_control()
{
	if(ble_data.X==1)
	{
		Wheel.PID_Sign=true;
	}
	else if(ble_data.Y==1)
	{
		Wheel.PID_Sign=false;
	}
}

void BLE_wheel_control()
{
	if(ble_data.L_X>-200&&ble_data.L_X<200)	ble_data.L_X=0;
	if(ble_data.L_Y>-200&&ble_data.L_Y<200)	ble_data.L_Y=0;

	if(ble_data.L_3==1||(Wheel.Close_sign==false&&ble_data.check==0))	
	{
		Wheel_Close();
	}
	else if(ble_data.L_X!=0||ble_data.L_Y!=0||ble_data.L_2>0||ble_data.R_2>0)
	{
		Wheel_Group_Closed_Loop_Control(ble_data.L_X/600.0, ble_data.L_Y/600.0, (ble_data.L_2-ble_data.R_2)/200.0,255);
	}
	else if(ble_data.left==1)
	{
		Wheel_Group_Closed_Loop_Control(-1, 0, 0,255);
	}
	else if(ble_data.right==1)
	{
		Wheel_Group_Closed_Loop_Control(1, 0, 0,255);
	}
	else if(ble_data.up==1)
	{
		Wheel_Group_Closed_Loop_Control(0, 1, 0,255);
	}
	else if(ble_data.down==1)
	{
		Wheel_Group_Closed_Loop_Control(0, -1, 0,255);
	}
}

void BLE_control()
{
	if(PID[0].debug_sign == true)
	{
		if(PID[0].BLE_debug_sign == true)
		{
			PID_Debug_Set_BLE(0);
		}
		PID_Debug_Speed(PID[0].debug_set);
	}
	else if(PID[1].debug_sign == true)
	{
		if(PID[1].BLE_debug_sign == true)
		{
			PID_Debug_Set_BLE(1);
		}
		PID_Debug_Position(PID[1].debug_set);
	}
	else if(Wheel.debug_sign==false)
	{
		BLE_PID_control();
		BLE_wheel_control();
	}
}

void BLE_State_Display()
{
	HAL_Delay(1);
    if(HAL_GPIO_ReadPin(BLE_STATE_GPIO_Port,BLE_STATE_Pin)==GPIO_PIN_SET)
    {
		HAL_TIM_Base_Stop_IT(&htim5);
	    lv_label_set_text(lv_obj_BLE.lable.BLE,"手柄未连接");
    }
    else
    {
		HAL_TIM_Base_Start_IT(&htim5);
	    lv_label_set_text(lv_obj_BLE.lable.BLE,"手柄已连接");
    }
}

void BLE_Init()
{
	BLE_Arm.x = Arm_Coord_X_0;
	BLE_Arm.y = Arm_Coord_Y_0;
	BLE_Arm.z = Arm_Coord_Z_0;
}