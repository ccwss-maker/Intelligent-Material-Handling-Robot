#include <display.h>
#include <arm.h>
#include <key.h>
#include <wheel.h>
#include <imu.h>
char str_buff[64];
u8g2_t u8g2;
uint8_t page=1;
mouse_information_ mouse_information[4];

extern key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/
extern arm_ arm;
extern message_imu_ message_imu;
extern float SPD_FL,SPD_FR,SPD_BL,SPD_BR;
extern uint8_t Tracking_Data[4];
/*u8g2函数*/
uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch(msg)
		{
		case U8X8_MSG_DELAY_MILLI:
		HAL_Delay(arg_int);
		break;
		
		case U8X8_MSG_DELAY_10MICRO:
		for (uint16_t n = 0; n < 320; n++)
		{
			__NOP();
		}
		break;
		
		case U8X8_MSG_DELAY_100NANO:
		__NOP();
		break;
		
		// Function to define the logic level of the CS line
		case U8X8_MSG_GPIO_CS:
		break;
		
		//Function to define the logic level of the Data/ Command line
		case U8X8_MSG_GPIO_DC:
			if (arg_int) HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin, GPIO_PIN_RESET);
		break;
		
		//Function to define the logic level of the RESET line
		case U8X8_MSG_GPIO_RESET:
			if (arg_int) HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin, GPIO_PIN_RESET);
		break;
		
		default:
			return 0; //A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}

/*显示字符串*/
void display_str(char* s,int x,int y,int sign)
{
	if(sign==1)
	{
	u8g2_ClearBuffer(&u8g2);			
	}
	u8g2_DrawStr(&u8g2,x,y,s);     
	u8g2_SendBuffer(&u8g2);
}

void OLED_display()
{
	if(key_pressed.BL[1])
	{
		if(page!=1)	page--;
		else				page=3;
		key_pressed.BL[1]=0;
	}
	else if(key_pressed.BR[1])
	{
		if(page!=3)	page++;
		else				page=1;
		key_pressed.BR[1]=0;
	}
	
	switch(page)
	{
		case 1:
			page_1_update_all();
			break;
		case 2:
			page_2_update_all();
			break;
		case 3:
			page_3_update_all();
			break;
//		case 4:
//			page_4_update_all();
	}
}

/*第一页界面初始化*/
void page_1_init()
{
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_7x13_mf);
	display_str("ARM_XYZ",40,9,1);
	u8g2_DrawLine(&u8g2, 	0,11,125,11);
	u8g2_DrawLine(&u8g2 , 0,27,125,27);
	u8g2_DrawLine(&u8g2 , 0,45,125,45);
	u8g2_DrawLine(&u8g2 , 0,63,125,63);
	
	u8g2_DrawLine(&u8g2 , 0,11,0,63);
	u8g2_DrawLine(&u8g2 , 9,11,9,63);
	u8g2_DrawLine(&u8g2 , 63,11,63,63);
	u8g2_DrawLine(&u8g2 , 72,11,72,63);
	u8g2_DrawLine(&u8g2 , 125,11,125,63);
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	display_str("x",1,23,0);
	display_str("y",1,41,0);
	display_str("z",1,59,0);
	display_str("a",64,23,0);
	display_str("b",64,41,0);
	display_str("c",64,59,0);
}

/*第一页更新*/
void page_1_update_all()
{
	static bool sign=true;
	if(sign)
	{
		page_1_init();
		sign=false;
	}
	
	if(key_pressed.sign[0])
	{
		mouse_test(&mouse_information[0], 1, 3);
		page_1_init();
		key_pressed.sign[0]=false;
	}
	update_mouse_page();
	update_mouse_1(&mouse_information[0]);
	update_mouse_2(&mouse_information[0],17,80,19,35,53);
	page_1_update_num();
	u8g2_SendBuffer(&u8g2);
}
extern PID_	PID;
/*第一页数值更新*/
void page_1_update_num()
{
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	
	sprintf((char*)str_buff,"%02x",Tracking_Data[0]);  //arm.x.value//PID.Kp
	u8g2_DrawStr(&u8g2,24,23,(char*)str_buff); 
	
	sprintf((char*)str_buff,"%02x",Tracking_Data[1]);	//arm.y.value//PID.Ki
	u8g2_DrawStr(&u8g2,24,41,(char*)str_buff);   
	
	sprintf((char*)str_buff,"%4.3f",PID.Kd);	//arm.z.value//PID.Kd
	u8g2_DrawStr(&u8g2,24,59,(char*)str_buff);
	
	sprintf((char*)str_buff,"%5.1f",message_imu.Roll);						//arm.a
	u8g2_DrawStr(&u8g2,78,23,(char*)str_buff);
	
	sprintf((char*)str_buff,"%5.1f",message_imu.Pitch);						//arm.b
	u8g2_DrawStr(&u8g2,78,41,(char*)str_buff);
	
	sprintf((char*)str_buff,"%5.1f",message_imu.Yaw);						//arm.C
	u8g2_DrawStr(&u8g2,78,59,(char*)str_buff);
}

/*第二页界面初始化*/
void page_2_init()
{
	u8g2_ClearBuffer(&u8g2);
	
	u8g2_SetFont(&u8g2, u8g2_font_6x13_mf);
	display_str("Wheel_SPD",37,9,1);
	u8g2_DrawLine(&u8g2, 0,11,125,11);
	u8g2_DrawLine(&u8g2 , 0,37,125,37);
	u8g2_DrawLine(&u8g2 , 0,63,125,63);
	
	u8g2_DrawLine(&u8g2 , 0,11,0,63);
	u8g2_DrawLine(&u8g2 , 9,11,9,63);
	u8g2_DrawLine(&u8g2 , 63,11,63,63);
	u8g2_DrawLine(&u8g2 , 72,11,72,63);
	u8g2_DrawLine(&u8g2 , 125,11,125,63);
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	display_str("F",1,23,0);
	display_str("L",1,35,0);
	display_str("B",1,49,0);
	display_str("L",1,61,0);
	
	display_str("F",64,23,0);
	display_str("R",64,35,0);
	display_str("B",64,49,0);
	display_str("R",64,61,0);

	u8g2_SendBuffer(&u8g2);
}

/*第二页更新*/
void page_2_update_all()
{
	if(key_pressed.sign[0])
	{
		mouse_test(&mouse_information[1], 2, 2);
		page_2_init();
		key_pressed.sign[0]=false;
	}
	update_mouse_page();
	update_mouse_1(&mouse_information[1]);
	update_mouse_2(&mouse_information[1],17,80,24,50,0);
	page_2_update_num();
	u8g2_SendBuffer(&u8g2);
}

/*第二页数值更新*/
void page_2_update_num()
{
	u8g2_SetFont(&u8g2, u8g2_font_7x13_mf);
	
	sprintf((char*)str_buff,"%5.1f",SPD_FL);
	u8g2_DrawStr(&u8g2,26,23,(char*)str_buff); 
//	sprintf((char*)str_buff,"%5.1f",speed_FL.Coder);
	u8g2_DrawStr(&u8g2,26,34,(char*)str_buff);
	
	sprintf((char*)str_buff,"%5.1f",SPD_BL);
	u8g2_DrawStr(&u8g2,26,49,(char*)str_buff);
//	sprintf((char*)str_buff,"%5.1f",speed_BL.Coder);
	u8g2_DrawStr(&u8g2,26,60,(char*)str_buff);
	
	sprintf((char*)str_buff,"%5.1f",SPD_FR);
	u8g2_DrawStr(&u8g2,89,23,(char*)str_buff);
//	sprintf((char*)str_buff,"%5.1f",speed_FR.Coder);
	u8g2_DrawStr(&u8g2,89,34,(char*)str_buff);  
	
	sprintf((char*)str_buff,"%5.1f",SPD_BR);
	u8g2_DrawStr(&u8g2,89,49,(char*)str_buff);
//	sprintf((char*)str_buff,"%5.1f",speed_BR.Coder);
	u8g2_DrawStr(&u8g2,89,60,(char*)str_buff);
}

/*第三页界面初始化*/
void page_3_init()
{
	u8g2_ClearBuffer(&u8g2);
	
	u8g2_SetFont(&u8g2, u8g2_font_7x13_mf);
	display_str("Arm_NUM",40,9,1);
	u8g2_DrawLine(&u8g2, 0,11,125,11);
	u8g2_DrawLine(&u8g2 , 0,27,125,27);
	u8g2_DrawLine(&u8g2 , 0,45,125,45);
	u8g2_DrawLine(&u8g2 , 0,63,125,63);
	
	u8g2_DrawLine(&u8g2 , 0,11,0,63);
	u8g2_DrawLine(&u8g2 , 9,11,9,63);
	u8g2_DrawLine(&u8g2 , 63,11,63,63);
	u8g2_DrawLine(&u8g2 , 72,11,72,63);
	u8g2_DrawLine(&u8g2 , 125,11,125,63);
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	display_str("1",1,23,0);
	display_str("2",1,41,0);
	display_str("3",1,59,0);
	display_str("4",64,23,0);
	display_str("5",64,41,0);
	display_str("6",64,59,0);
}

/*第三页更新*/
void page_3_update_all()
{
	if(key_pressed.sign[0])
	{
		mouse_test(&mouse_information[2],2,3);
		page_3_init();
		key_pressed.sign[0]=false;
	}
	update_mouse_page();
	update_mouse_1(&mouse_information[2]);
	update_mouse_2(&mouse_information[2],17,80,19,35,53);
	page_3_update_num();
	u8g2_SendBuffer(&u8g2);
}

/*第三页数值更新*/
void page_3_update_num()
{
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	
	sprintf((char*)str_buff,"%4d",PWM_JXB_1);
	u8g2_DrawStr(&u8g2,24,23,(char*)str_buff); 
	
	sprintf((char*)str_buff,"%4d",PWM_JXB_2);
	u8g2_DrawStr(&u8g2,24,41,(char*)str_buff);   
	
	sprintf((char*)str_buff,"%4d",PWM_JXB_3);
	u8g2_DrawStr(&u8g2,24,59,(char*)str_buff);
	
	sprintf((char*)str_buff,"%4d",PWM_JXB_4);
	u8g2_DrawStr(&u8g2,87,23,(char*)str_buff);
	
	sprintf((char*)str_buff,"%4d",PWM_JXB_5);
	u8g2_DrawStr(&u8g2,87,41,(char*)str_buff);
	
	sprintf((char*)str_buff,"%4d",PWM_JXB_6);
	u8g2_DrawStr(&u8g2,87,59,(char*)str_buff);
}

///*第四页界面初始化*/
//void page_4_init()
//{
//	u8g2_ClearBuffer(&u8g2);
//	
//	u8g2_SetFont(&u8g2, u8g2_font_6x13_mf);
//	display_str("Wheel_NUM",37,9,1);
//	u8g2_DrawLine(&u8g2, 0,11,125,11);
//	u8g2_DrawLine(&u8g2 , 0,37,125,37);
//	u8g2_DrawLine(&u8g2 , 0,63,125,63);
//	
//	u8g2_DrawLine(&u8g2 , 0,11,0,63);
//	u8g2_DrawLine(&u8g2 , 9,11,9,63);
//	u8g2_DrawLine(&u8g2 , 63,11,63,63);
//	u8g2_DrawLine(&u8g2 , 72,11,72,63);
//	u8g2_DrawLine(&u8g2 , 125,11,125,63);
//	
//	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
//	display_str("F",1,23,0);
//	display_str("L",1,35,0);
//	display_str("B",1,49,0);
//	display_str("L",1,61,0);
//	
//	display_str("F",64,23,0);
//	display_str("R",64,35,0);
//	display_str("B",64,49,0);
//	display_str("R",64,61,0);

//	u8g2_SendBuffer(&u8g2);
//}

///*第四页更新*/
//void page_4_update_all(key_pressed_ *key_pressed_p)
//{
//	if(key_pressed.sign[0])
//	{
//		mouse_test(key_pressed_p, &mouse_information[3],2,2);
//		page_4_init();
//		key_pressed.sign[0]=false;
//	}
//	update_mouse_page();
//	update_mouse_1(&mouse_information[3]);
//	update_mouse_2(&mouse_information[3],17,80,24,50,0);
//	page_4_update_num();
//	u8g2_SendBuffer(&u8g2);
//}

///*第四页数值更新*/
//void page_4_update_num()
//{
//	u8g2_SetFont(&u8g2, u8g2_font_7x13_mf);
//	
//	sprintf((char*)str_buff,"%5d",PWM_FL);
//	u8g2_DrawStr(&u8g2,26,23,(char*)str_buff); 
//	sprintf((char*)str_buff,"%5.1f",speed_FL.Coder);
//	u8g2_DrawStr(&u8g2,26,34,(char*)str_buff);
//	
//	sprintf((char*)str_buff,"%5d",PWM_BL);
//	u8g2_DrawStr(&u8g2,26,49,(char*)str_buff);
//	sprintf((char*)str_buff,"%5.1f",speed_BL.Coder);
//	u8g2_DrawStr(&u8g2,26,60,(char*)str_buff);
//	
//	sprintf((char*)str_buff,"%5d",PWM_FR);
//	u8g2_DrawStr(&u8g2,89,23,(char*)str_buff);
//	sprintf((char*)str_buff,"%5.1f",speed_FR.Coder);
//	u8g2_DrawStr(&u8g2,89,34,(char*)str_buff);  
//	
//	sprintf((char*)str_buff,"%5d",PWM_BR);
//	u8g2_DrawStr(&u8g2,89,49,(char*)str_buff);
//	sprintf((char*)str_buff,"%5.1f",speed_BR.Coder);
//	u8g2_DrawStr(&u8g2,89,60,(char*)str_buff);
//}

/*光标功能检测*/
void mouse_test(mouse_information_* mouse_information, uint8_t x, uint8_t y)
{
	if(!mouse_information->selected)
	{
		if(key_pressed.T[1]==1)
		{
			if(mouse_information->mouse_2[1]==0)	mouse_information->mouse_2[1]=y-1;
			else	mouse_information->mouse_2[1]--;
			key_pressed.T[1]=0;
		}
		else if(key_pressed.B[1]==1)
		{
			if(mouse_information->mouse_2[1]==y-1)	mouse_information->mouse_2[1]=0;
			else 	mouse_information->mouse_2[1]++;
			key_pressed.B[1]=0;
		}
		else if(key_pressed.L[1]==1)
		{
			if(mouse_information->mouse_2[0]==0)	mouse_information->mouse_2[0]=x-1;
			else 	mouse_information->mouse_2[0]--;
			key_pressed.L[1]=0;
		}
		else if(key_pressed.R[1]==1)
		{
			if(mouse_information->mouse_2[0]==x-1)	mouse_information->mouse_2[0]=0;
			else 	mouse_information->mouse_2[0]++;
			key_pressed.R[1]=0;
		}
	}
	
	if(key_pressed.C[1]==1)
	{
		if(mouse_information->selected)	mouse_information->selected=false;
		else														mouse_information->selected=true;
		key_pressed.C[1]=0;
	}
	else if(key_pressed.TL[1]==1)
	{
		if(mouse_information->mouse_1!=3)	mouse_information->mouse_1++;
		else	mouse_information->mouse_1=0;
		key_pressed.TL[1]=0;
	}
	else if(key_pressed.TR[1]==1)
	{
		if(mouse_information->mouse_1!=0)	mouse_information->mouse_1--;
		else	mouse_information->mouse_1=3;
		key_pressed.TR[1]=0;
	}
}

/*页面光标更新*/
void update_mouse_page()
{
	static uint8_t x = 0;
	switch(page)
	{
		case 1:
			x = 3;
			break;
		case 2:
			x = 12;
			break;
		case 3:
			x = 21;
			break;
		case 4:
			x = 30;
			break;
	}
	u8g2_DrawCircle(&u8g2,3,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,12,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,21,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,30,4,3,U8G2_DRAW_ALL);
	u8g2_DrawDisc(&u8g2,x,4,3,U8G2_DRAW_ALL);
}

/*光标1更新*/
void update_mouse_1(mouse_information_* mouse_information)
{
	static uint8_t x = 0;
	switch(mouse_information->mouse_1)
	{
		case 0:
			x = 122;
			break;
		case 1:
			x = 113;
			break;
		case 2:
			x = 104;
			break;
		case 3:
			x = 95;
			break;
	}
	u8g2_DrawCircle(&u8g2,95,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,104,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,113,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,122,4,3,U8G2_DRAW_ALL);
	u8g2_DrawDisc(&u8g2,x,4,3,U8G2_DRAW_ALL);
}
/*光标2更新*/
void update_mouse_2(mouse_information_* mouse_information, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint8_t y2)
{
	static uint8_t x = 0 , y = 0 , rad = 2;

	switch(mouse_information->mouse_2[0])
	{
		case 0:
			x = x0;
			break;
		case 1:
			x = x1;
			break;
	}
	
	switch(mouse_information->mouse_2[1])
	{
		case 0:
			y = y0;
			break;
		case 1:
			y = y1;
			break;
		case 2:
			y = y2;
			break;
	}

	if(mouse_information->selected)	rad = 3;
	else														rad = 2;
	u8g2_DrawDisc(&u8g2,x,y,rad,U8G2_DRAW_ALL);
}
