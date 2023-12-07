#include "ble.h"
#include "lv_app.h"
#include "stdio.h"
#include "wheel.h"
#include "PID.h"
#include "lv_app_wheel.h"
#include "lv_app_pid.h"
//#include <arm.h>
//#include <key.h>
ble_data_ ble_data;
ble_uart_ ble_uart;
// extern uint8_t Wheel_PID_Sign;
// extern key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/
// extern arm_ arm;
// #if PID_Config_Mode
// extern PID_	PID[2];
// extern float www;
// #endif
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
    
    uint8_t data[10]={0};
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

// void BLE_arm_control()
// {
// 	if(ble_data.R_X>100)	
// 	{
// 		key_pressed.B[0]=2;
// 		Control_Angle_ble(0);
// 		key_pressed.B[0]=0;
// 	}
// 	else if(ble_data.R_X<-100)
// 	{
// 		key_pressed.T[0]=2;
// 		Control_Angle_ble(0);
// 		key_pressed.T[0]=0;
// 	}
	
// 	if(ble_data.R_Y>100)	
// 	{
// 		key_pressed.T[0]=3;
// 		Control_Angle_ble(1);
// 		key_pressed.T[0]=0;
// 	}
// 	else if(ble_data.R_Y<-100)
// 	{
// 		key_pressed.B[0]=3;
// 		Control_Angle_ble(1);
// 		key_pressed.B[0]=0;
// 	}
	
// 	if(ble_data.L_1==1&&ble_data.R_1==0)	
// 	{
// 		key_pressed.T[0]=3;
// 		Control_Angle_ble(2);
// 		key_pressed.T[0]=0;
// 	}
// 	else if(ble_data.R_1==1&&ble_data.L_1==0)
// 	{
// 		key_pressed.B[0]=3;
// 		Control_Angle_ble(2);
// 		key_pressed.B[0]=0;
// 	}
	
// 	if(ble_data.A==1&&ble_data.B==0)
// 	{
// 		arm_open();
// 	}
// 	else if(ble_data.A==0&&ble_data.B==1)
// 	{
// 		arm_close();
// 	}
	
// 	if(ble_data.R_3==1)
// 	{
// 		arm_Init();
// 	}
// }

void BLE_value_control()
{
	if(ble_data.L_3==1)
	{
		if(ble_data.L_Y>1000)
		{
			Wheel.debug_sign=false;
        	Wheel_slider_clear();
			lv_obj_clear_state(lv_obj_WHEEL.sw_debug,LV_STATE_CHECKED);
			lv_obj_clear_state(lv_obj_WHEEL.sw_power,LV_STATE_CHECKED);
			lv_obj_WHEEL_Control(false,false);
        	Wheel_Close();
		}
		else if(ble_data.L_Y<-1000)
		{
			Wheel.debug_sign=true;
			lv_obj_add_state(lv_obj_WHEEL.sw_debug,LV_STATE_CHECKED);
			lv_obj_WHEEL_Control(true,false);
		}
	}

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
		BLE_value_control();
		// BLE_wheel_control();
	}
}
void BLE_State_Display()
{
	HAL_Delay(1);
    if(HAL_GPIO_ReadPin(BLE_STATE_GPIO_Port,BLE_STATE_Pin)==GPIO_PIN_SET)
    {
	    lv_label_set_text(lv_obj_BLE.lable.BLE,"手柄未连接");
    }
    else
    {
	    lv_label_set_text(lv_obj_BLE.lable.BLE,"手柄已连接");
    }
}