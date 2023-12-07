#include <ble.h>
#include <wheel.h>
#include <arm.h>
#include <key.h>
ble_data_ ble_data;
ble_uart_ ble_uart;
extern uint8_t Wheel_PID_Sign;
extern key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/
extern arm_ arm;
#if PID_Config_Mode
extern PID_	PID[2];
extern float www;
#endif
void BLE_Move_Receive()
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
	
	for(int i=0;i<12;i++)
	{
		if((ble_data_check.digital_data>>i&(int16_t)1)==1)	*(&(ble_data_check.L_3)+i)=1;
		else																								*(&(ble_data_check.L_3)+i)=0;
	}
	memcpy(&ble_data,&ble_data_check,sizeof(ble_data_check));
	ble_data.break_sign=true;
}


void BLE_arm_control()
{
	if(ble_data.R_X>100)	
	{
		key_pressed.B[0]=2;
		Control_Angle_ble(0);
		key_pressed.B[0]=0;
	}
	else if(ble_data.R_X<-100)
	{
		key_pressed.T[0]=2;
		Control_Angle_ble(0);
		key_pressed.T[0]=0;
	}
	
	if(ble_data.R_Y>100)	
	{
		key_pressed.T[0]=3;
		Control_Angle_ble(1);
		key_pressed.T[0]=0;
	}
	else if(ble_data.R_Y<-100)
	{
		key_pressed.B[0]=3;
		Control_Angle_ble(1);
		key_pressed.B[0]=0;
	}
	
	if(ble_data.L_1==1&&ble_data.R_1==0)	
	{
		key_pressed.T[0]=3;
		Control_Angle_ble(2);
		key_pressed.T[0]=0;
	}
	else if(ble_data.R_1==1&&ble_data.L_1==0)
	{
		key_pressed.B[0]=3;
		Control_Angle_ble(2);
		key_pressed.B[0]=0;
	}
	
	if(ble_data.A==1&&ble_data.B==0)
	{
		arm_open();
	}
	else if(ble_data.A==0&&ble_data.B==1)
	{
		arm_close();
	}
	
	if(ble_data.R_3==1)
	{
		arm_Init();
	}
}

void BLE_wheel_control()
{
	if(ble_data.L_X>-200&&ble_data.L_X<200)	ble_data.L_X=0;
	if(ble_data.L_Y>-200&&ble_data.L_Y<200)	ble_data.L_Y=0;

	if(ble_data.check==0||ble_data.L_3==1)		
	{
		wheel_close();
	}
	else if(ble_data.L_X!=0||ble_data.L_Y!=0||(ble_data.L_2>100&&ble_data.R_2<100)||(ble_data.R_2>100&&ble_data.L_2<100))
	{
		Control_Mecanum(-ble_data.L_X/600.0, ble_data.L_Y/600.0, (ble_data.L_2-ble_data.R_2)/3000.0,255);
	}
	else if(ble_data.left==1)
	{
		Control_Mecanum(1, 0, 0,255);
	}
	else if(ble_data.right==1)
	{
		Control_Mecanum(-1, 0, 0,255);
	}
	else if(ble_data.up==1)
	{
		Control_Mecanum(0, 1, 0,255);
	}
	else if(ble_data.down==1)
	{
		Control_Mecanum(0, -1, 0,255);
	}
	
	if(ble_data.X==1)
	{
		Wheel_PID_Sign=1;
	}
	else if(ble_data.Y==1)
	{
		Wheel_PID_Sign=0;
	}
}

void BLE_control()
{
	BLE_arm_control();
	BLE_wheel_control();
}


void BLE_PID_Receive()
{
	char receive[ble_uart_buffer]={0};
	memcpy(receive,ble_uart.receive,ble_uart.rx_len);

	if(receive[0]!='#')	return;
	
	char *s_num[10]={NULL};
	float num[10]={0};
	uint8_t data_num=0;
	
	for(uint8_t i=0;i<ble_uart.rx_len;i++)
	{
		char* str=strchr((char*)receive+i,',');
		if(str!=NULL&&(data_num==0||str!=s_num[data_num-1]))
		{
			s_num[data_num++]=str;
		}
	}
	s_num[data_num]=s_num[0]+ble_uart.rx_len-1;
	if(s_num[1]==NULL)	
		return;
	
	for(int i=0;i<data_num;i++)
	{
		char data[10]={0};
		memcpy(data,s_num[i]+1,s_num[i+1]-s_num[i]-1);
		num[i]=atof(data);
	}
	

	#if PID_Config_Mode
	if(data_num==8)
	{
		PID_clear(&PID[0]);
		PID_clear(&PID[1]);
		extern float www;
		extern float yaw;
		yaw=num[0];
//		memcpy(&PID[0],&num[1],7);
		PID[1].Kp=num[1];
		PID[1].Ki=num[2];
		PID[1].Kd=num[3];
		PID[1].max_iout=num[4];
		PID[1].max_out=num[5];
		PID[1].Integral_Separation_Threshold=num[6];
		PID[1].Dead_Zone=num[7];
	}
	#endif
}