#include <ble.h>
#include <wheel.h>
#include <arm.h>
#include <key.h>
message_ message;
key_ key;
uint8_t receive[8];
extern key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/
extern arm_ arm;
void BLE_Receive()
{
	if(receive[0]=='#')
	{
		message.function=receive[1];
		message.num=receive[2]<<8;
		message.num|=receive[3];
		message.check=receive[0]^receive[1]^receive[2]^receive[3];
		if(receive[4]!=message.check)
			return;
	}
	
	switch(message.function)
	{
		case 0:/*LX*/
			key.L_X=message.num;
			break;
		case 1:/*LY*/
			key.L_Y=-message.num;
			break;
		case 2:/*L3*/
			key.L_3=message.num;
			break;
		case 3:/*L2*/
			key.L_2=message.num;
			break;
		case 4:/*L1*/
			key.L_1=message.num;
			break;
		case 5:/*RX*/
			key.R_X=message.num;
			break;
		case 6:/*RY*/
			key.R_Y=-message.num;
			break;
		case 7:/*R3*/
			key.R_3=message.num;
			break;
		case 8:/*R2*/
			key.R_2=message.num;
			break;
		case 9:/*R1*/
			key.R_1=message.num;
			break;
		case 10:/*up*/
			key.up=message.num;
			break;
		case 11:/*down*/
			key.down=message.num;
			break;
		case 12:/*left*/
			key.left=message.num;
			break;
		case 13:/*right*/
			key.right=message.num;
			break;
		case 14:/*A*/
			key.A=message.num;
			break;
		case 15:/*B*/
			key.B=message.num;
			break;
		case 16:/*X*/
			key.X=message.num;
			break;
		case 17:/*Y*/
			key.Y=message.num;
			break;
		case 18:/*NONE*/
			key.none=message.num;
			break;
	}
//	Control_Mecanum(key_.L_X/1800.0,key_.L_Y/1800.0,(key_.R_2-key_.L_2)/3840.0);
}


void BLE_arm_control()
{
	if(key.R_X>100)	
	{
		key_pressed.B[0]=2;
		Control_Angle_ble(0);
		key_pressed.B[0]=0;
	}
	else if(key.R_X<-100)
	{
		key_pressed.T[0]=2;
		Control_Angle_ble(0);
		key_pressed.T[0]=0;
	}
	
	if(key.R_Y>100)	
	{
		key_pressed.T[0]=3;
		Control_Angle_ble(1);
		key_pressed.T[0]=0;
	}
	else if(key.R_Y<-100)
	{
		key_pressed.B[0]=3;
		Control_Angle_ble(1);
		key_pressed.B[0]=0;
	}
	
	if(key.L_1==1&&key.R_1==0)	
	{
		key_pressed.T[0]=3;
		Control_Angle_ble(2);
		key_pressed.T[0]=0;
	}
	else if(key.R_1==1&&key.L_1==0)
	{
		key_pressed.B[0]=3;
		Control_Angle_ble(2);
		key_pressed.B[0]=0;
	}
	
	if(key.A==1&&key.B==0)
	{
		arm_open();
	}
	else if(key.A==0&&key.B==1)
	{
		arm_close();
	}
	
	if(key.R_3==1)
	{
		arm_Init();
	}
}

void BLE_wheel_control()
{
	if(key.L_X>-200&&key.L_X<200)	key.L_X=0;
	if(key.L_Y>-200&&key.L_Y<200)	key.L_Y=0;

	if(key.L_X!=0||key.L_Y!=0||(key.L_2>100&&key.R_2<100)||(key.R_2>100&&key.L_2<100))
	{
		Control_Mecanum(-key.L_X/600.0, key.L_Y/600.0, (key.L_2-key.R_2)/900.0,255);
	}
	else if(key.left==1)
	{
		Control_Mecanum(1, 0, 0,255);
	}
	else if(key.right==1)
	{
		Control_Mecanum(-1, 0, 0,255);
	}
	else if(key.up==1)
	{
		Control_Mecanum(0, 1, 0,255);
	}
	else if(key.down==1)
	{
		Control_Mecanum(0, -1, 0,255);
	}
	else if(key.none==0)	
	{
		wheel_close();
	}
}

void BLE_control()
{
	BLE_arm_control();
	BLE_wheel_control();
}
