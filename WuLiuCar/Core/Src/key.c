#include <key.h>

key_pressed_ key_pressed;	/*TL L BL T C B TR R BR*/
bool sign[9]={false};

void GPIO_test(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t key[], bool* sign)
{
  assert_param(IS_GPIO_PIN(GPIO_Pin));

	if((GPIOx->IDR & GPIO_Pin) == (uint32_t)GPIO_PIN_RESET)
	{
		if(htim2.State==HAL_TIM_STATE_READY)
		{
			key[1] = 1; 
			memset(key_pressed.sign,true,sizeof(key_pressed.sign));
			HAL_TIM_Base_Start(&htim2);
			TIM2->CNT = 0;
		}
		else if(htim2.State==HAL_TIM_STATE_BUSY)
		{
			if(TIM2->CNT < 8000)				key[0] = 1; 
			else if(TIM2->CNT < 16000)	key[0] = 2; 
			else												key[0] = 3; 
		}
		*sign=true;
	}
	else if(*sign&&htim2.State==HAL_TIM_STATE_BUSY)
	{
		*sign=false;
		HAL_TIM_Base_Stop(&htim2);
		memset(key,0,2);
		memset(key_pressed.sign,false,sizeof(key_pressed.sign));
	}
}

void key_test()
{
	GPIO_test(KEY_TL_GPIO_Port,KEY_TL_Pin,key_pressed.TL,&sign[0]);
	GPIO_test(KEY_L_GPIO_Port,KEY_L_Pin,key_pressed.L,&sign[1]);
	GPIO_test(KEY_BL_GPIO_Port,KEY_BL_Pin,key_pressed.BL,&sign[2]);
	GPIO_test(KEY_T_GPIO_Port,KEY_T_Pin,key_pressed.T,&sign[3]);
	GPIO_test(KEY_C_GPIO_Port,KEY_C_Pin,key_pressed.C,&sign[4]);
	GPIO_test(KEY_B_GPIO_Port,KEY_B_Pin,key_pressed.B,&sign[5]);
	GPIO_test(KEY_TR_GPIO_Port,KEY_TR_Pin,key_pressed.TR,&sign[6]);
	GPIO_test(KEY_R_GPIO_Port,KEY_R_Pin,key_pressed.R,&sign[7]);
	GPIO_test(KEY_BR_GPIO_Port,KEY_BR_Pin,key_pressed.BR,&sign[8]);
}