#include <arm.h>

arm_ arm={{0,0,-150,150},{96,76,20,150},{40,0,40,140}};
float l1=87.6,l2=62.5;
extern key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/


void Control_NUM(mouse_information_ mouse_information, uint8_t page)
{
	if(page==3&&mouse_information.selected&&key_pressed.sign[1])
	{
		if(!mouse_information.mouse_2[0])
		{
			switch(mouse_information.mouse_2[1])
			{
				case 0:
					Control_NUM_(&PWM_JXB_1,mouse_information.mouse_1,1300,1850);
					break;
				case 1:
					Control_NUM_(&PWM_JXB_2,mouse_information.mouse_1,500,2500);
					break;
				case 2:
					Control_NUM_(&PWM_JXB_3,mouse_information.mouse_1,500,2200);
					break;
			}
		}
		else
		{
			switch(mouse_information.mouse_2[1])
			{
				case 0:
					Control_NUM_(&PWM_JXB_4,mouse_information.mouse_1,800,2500);
					break;
				case 1:
					Control_NUM_(&PWM_JXB_5,mouse_information.mouse_1,500,2500);
					break;
				case 2:
					Control_NUM_(&PWM_JXB_6,mouse_information.mouse_1,500,2500);
					break;
			}
		}
	}
}

void Control_NUM_(volatile uint32_t* arm_p, uint8_t mode, int min, int max)
{
	if(key_pressed.T[0])			Control_NUM_add_sub(arm_p, key_pressed.T[0], mode, max,1);
	else if(key_pressed.B[0])	Control_NUM_add_sub(arm_p, key_pressed.B[0], mode, min,-1);
	else if(key_pressed.L[0])	Control_NUM_add_sub(arm_p, key_pressed.L[0], mode, min,-1);
	else if(key_pressed.R[0])	Control_NUM_add_sub(arm_p, key_pressed.R[0], mode, max,1);
}

void Control_NUM_add_sub(volatile uint32_t* arm_p, uint8_t key, uint8_t mode, int max_min, int sign)
{
	switch(mode)
	{
		case 0:
			switch(key)
			{
				case 1:
					if(sign*(*arm_p+sign*ARM_ADD_SUB_1-max_min) <= 0)	*arm_p +=sign*ARM_ADD_SUB_1;
					else								*arm_p = max_min;
					break;
				case 2:
					if(sign*(*arm_p+sign*ARM_ADD_SUB_2-max_min) <= 0)	*arm_p +=sign*ARM_ADD_SUB_2;
					else								*arm_p = max_min;
					break;
				case 3:
					if(sign*(*arm_p+sign*ARM_ADD_SUB_3-max_min) <= 0)	*arm_p +=sign*ARM_ADD_SUB_3;
					else								*arm_p = max_min;
			}
			break;
		case 1:
			if(sign*(*arm_p+sign*ARM_ADD_SUB_1-max_min) <= 0)	*arm_p +=sign*ARM_ADD_SUB_1;
			else								*arm_p = max_min;
			break;
		case 2:
			if(sign*(*arm_p+sign*ARM_ADD_SUB_2-max_min) <= 0)	*arm_p +=sign*ARM_ADD_SUB_2;
			else								*arm_p = max_min;
			break;
		case 3:
			if(sign*(*arm_p+sign*ARM_ADD_SUB_3-max_min) <= 0)	*arm_p +=sign*ARM_ADD_SUB_3;
			else								*arm_p = max_min;
	}
}

void Control_Angle(mouse_information_ mouse_information, uint8_t page)
{
	static bool sign=true;
	if(sign)
	{
		Angle_calculate();
		sign=false;
	}
	if(page==1)
	{
		if(mouse_information.selected&&key_pressed.sign[1])
		{
			if(!mouse_information.mouse_2[0])
			{
				switch(mouse_information.mouse_2[1])
				{
					case 0:
						if(key_pressed.T[0])	
						{
							arm.x.value+=1;
						}
						else if(key_pressed.B[0])	
						{
							arm.x.value-=1;
						}
						Control_Angle_(&arm.x, &arm.y, &arm.z, mouse_information.mouse_1);
						break;
					case 1:
						if(key_pressed.T[0])	
						{
							arm.y.value+=1;
						}
						else if(key_pressed.B[0])	
						{
							arm.y.value-=1;
						}
						Control_Angle_(&arm.y, &arm.z, &arm.x, mouse_information.mouse_1);
						break;
					case 2:
						if(key_pressed.T[0])	
						{
							arm.z.value+=1;
						}
						else if(key_pressed.B[0])	
						{
							arm.z.value-=1;
						}
						Control_Angle_(&arm.z, &arm.y, &arm.x, mouse_information.mouse_1);
						break;
				}
			}
			Angle_calculate();
			Angle_move();
		}
	}
}

void Control_Angle_(value_* v1, value_* v2, value_* v3, uint8_t mode)
{
	if(key_pressed.T[0])			Control_Angle_add_sub(v1, v2, v3, key_pressed.T[0], mode, Control_Angle_add_);
	else if(key_pressed.B[0])	Control_Angle_add_sub(v1, v2, v3, key_pressed.B[0], mode, Control_Angle_sub_);
	else if(key_pressed.R[0])	Control_Angle_add_sub(v1, v2, v3, key_pressed.R[0], mode, Control_Angle_sub_);
	else if(key_pressed.L[0])	Control_Angle_add_sub(v1, v2, v3, key_pressed.L[0], mode, Control_Angle_add_);
}

void Control_Angle_add_sub(value_* v1, value_* v2, value_* v3, uint8_t key, uint8_t mode,void (*p)(value_* v1, value_* v2, value_* v3, int step, int delay))
{
	switch(mode)
	{
		case 0:
			switch(key)
			{
				case 1:
					(*p)(v1, v2, v3, 1, delay_1);
					break;
				case 2:
					(*p)(v1, v2, v3, 1, delay_2);
					break;
				case 3:
					(*p)(v1, v2, v3, 1, delay_3);
			}
			break;
		case 1:
			(*p)(v1, v2, v3, 1, delay_1);
			break;
		case 2:
			(*p)(v1, v2, v3, 1, delay_2);
			break;
		case 3:
			(*p)(v1, v2, v3, 1, delay_3);
	}
}

void Control_Angle_add_(value_* v1, value_* v2, value_* v3, int step, int delay)
{
	if(v1->value-v1->increment+step <= v1->max)
	{
		v1->value +=step;
		while(pow(pow(v1->value-v1->increment,2)+pow(v2->value-v2->increment,2),0.5)>(l1+l2))
		{
			if(v2->value-v2->increment-1>v2->min)	v2->value-=1;
			else break;
		}
		while(pow(pow(v1->value-v1->increment,2)+pow(v3->value-v3->increment,2),0.5)>(l1+l2))
		{
			if(v3->value-v3->increment-1>v3->min)	v3->value-=1;
			else break;
		}
		HAL_Delay(delay);
	}	
}

void Control_Angle_sub_(value_* v1, value_* v2, value_* v3, int step, int delay)
{	
	if(v1->value-v1->increment-step >= v1->min)
	{
		v1->value -=step;
		HAL_Delay(delay);
	}
}

void Angle_calculate()
{
	float x = arm.x.value-arm.x.increment;
	float y = arm.y.value-arm.y.increment;
	float z = arm.z.value-arm.z.increment;
	float x_ =pow(pow(x,2)+pow(y,2),0.5);
	arm.a=acos((pow(l1,2)-pow(l2,2)+pow(x_,2)+pow(z,2))/(2*l1*pow(pow(x_,2)+pow(z,2),0.5)))+atan(z/x_);
	arm.b=arm.a-asin((z-l1*sin(arm.a))/l2);
	arm.c=asin((z-l1*sin(arm.a))/l2);
	arm.d=atan(x/y);
	arm.a*=180/3.14;
	arm.b*=180/3.14;
	arm.c*=180/3.14;
	arm.d*=180/3.14;
}

void Angle_move_(volatile uint32_t* arm_p, float angle, int incremental)
{
	*arm_p=(int)(angle/180*2000+incremental);
}

void Angle_move()
{
	Angle_move_(&PWM_JXB_5,arm.d,1900);
	Angle_move_(&PWM_JXB_4,arm.a,500);
	Angle_move_(&PWM_JXB_3,arm.b,580);
	Angle_move_(&PWM_JXB_2,arm.c,1350);
}

void Control_Angle_ble(uint8_t change)
{
	switch(change)
	{
		case 0:
			Control_Angle_(&arm.x, &arm.y, &arm.z, 0);
			break;
		case 1:
			Control_Angle_(&arm.y, &arm.z, &arm.x, 0);
			break;
		case 2:
			Control_Angle_(&arm.z, &arm.y, &arm.x, 0);
			break;
	}
	Angle_calculate();
	Angle_move();
}

void arm_Init()
{
	arm.x.value=0;
	arm.y.value=96;
	arm.z.value=40;
	Angle_calculate();
	Angle_move();
	arm_open();
}

void arm_close()
{
	PWM_JXB_1=1300;
}

void arm_open()
{
	PWM_JXB_1=1700;
}

//void Control_Angle_(value_* v1, value_* v2, value_* v3, uint8_t mode)
//{
//	if(key_pressed.T[0])			Control_Angle_add_sub(v1, v2, v3, key_pressed.T[0], mode, Control_Angle_add_);
//	else if(key_pressed.B[0])	Control_Angle_add_sub(v1, v2, v3, key_pressed.B[0], mode, Control_Angle_sub_);
//	else if(key_pressed.R[0])	Control_Angle_add_sub(v1, v2, v3, key_pressed.R[0], mode, Control_Angle_sub_);
//	else if(key_pressed.L[0])	Control_Angle_add_sub(v1, v2, v3, key_pressed.L[0], mode, Control_Angle_add_);
//}
