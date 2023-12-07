/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
///////////////////////////////OLED显示/////////////////////////////	
#include "u8g2.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/////////////////////////////OLED显示/////////////////////////////
u8g2_t u8g2;
uint8_t str_buff[64];	
uint8_t option[2] = {1,1};
uint8_t option_change_sign = 0; //改变为1
uint8_t KEY_C_sign = 0; //按下一次为1
uint8_t UI_ARM_sign = 0; //机械臂为1
uint8_t change_speed_sign= 0; //数值调整速度标志 0：单次调整 1：慢速 2：中速 3：快速
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PWM_JXB_1 TIM2->CCR1
#define PWM_JXB_2 TIM2->CCR2
#define PWM_JXB_3 TIM2->CCR3
#define PWM_JXB_4 TIM3->CCR2
#define PWM_JXB_5 TIM3->CCR1
#define PWM_JXB_6 TIM2->CCR4

#define PWM_FL TIM1->CCR4
#define PWM_FR TIM1->CCR3
#define PWM_BL TIM1->CCR2
#define PWM_BR TIM1->CCR1

#define KEY_L HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_RESET
#define KEY_R HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)==GPIO_PIN_RESET
#define KEY_T HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)==GPIO_PIN_RESET
#define KEY_B HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)==GPIO_PIN_RESET
#define KEY_C HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)==GPIO_PIN_RESET
#define KEY_L_PIN GPIO_PIN_4
#define KEY_R_PIN GPIO_PIN_12
#define KEY_T_PIN GPIO_PIN_15
#define KEY_B_PIN GPIO_PIN_13
#define KEY_C_PIN GPIO_PIN_3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
///////////////////////////////OLED显示/////////////////////////////
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
			if (arg_int) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12, GPIO_PIN_RESET);
		break;
		
		//Function to define the logic level of the RESET line
		case U8X8_MSG_GPIO_RESET:
			if (arg_int) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, GPIO_PIN_RESET);
		break;
		
		default:
			return 0; //A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}

void display(char message[],int x,int y,int sign)
{
	if(sign==1)
	{
	u8g2_ClearBuffer(&u8g2);			
	}
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%s",message);
	u8g2_DrawStr(&u8g2,x,y,(char*)str_buff);     
	u8g2_SendBuffer(&u8g2);
}

void display_num(char message[],int num,int x,int y,int sign)
{
	if(sign==1)
	{
	u8g2_ClearBuffer(&u8g2);			
	}
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%s%d",message,num);
	u8g2_DrawStr(&u8g2,x,y,(char*)str_buff);     
	u8g2_SendBuffer(&u8g2);
}

void page_1_init()
{
	display("P1 ARM",40,9,1);
	u8g2_DrawLine(&u8g2 , 0,11,125,11);
	u8g2_DrawLine(&u8g2 , 0,27,125,27);
	u8g2_DrawLine(&u8g2 , 0,45,125,45);
	u8g2_DrawLine(&u8g2 , 0,63,125,63);
	
	u8g2_DrawLine(&u8g2 , 0,11,0,63);
	u8g2_DrawLine(&u8g2 , 9,11,9,63);
	u8g2_DrawLine(&u8g2 , 63,11,63,63);
	u8g2_DrawLine(&u8g2 , 72,11,72,63);
	u8g2_DrawLine(&u8g2 , 125,11,125,63);
	
	display("1",1,23,0);
	display("2",1,41,0);
	display("3",1,59,0);
	display("4",64,23,0);
	display("5",64,41,0);
	display("6",64,59,0);
	
	static uint8_t sign_once_1 = 1;
	if(sign_once_1)
	{
		u8g2_DrawCircle(&u8g2,4,4,3,U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2,13,4,3,U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2,22,4,3,U8G2_DRAW_ALL);
		u8g2_DrawCircle(&u8g2,31,4,3,U8G2_DRAW_ALL);
		
		u8g2_DrawDisc(&u8g2,31,4,3,U8G2_DRAW_ALL);
		u8g2_DrawDisc(&u8g2,17,19,2,U8G2_DRAW_ALL);
		sign_once_1=0;
	}
	
	u8g2_SendBuffer(&u8g2);
}

void page_1_circle_num()
{
	static uint8_t x = 0 , y = 0 , rad = 2;
	switch(option[0])
	{
		case 1:
			x = 17;
			break;
		case 2:
			x = 80;
			break;
	}
	
	switch(option[1])
	{
		case 1:
			y = 19;
			break;
		case 2:
			y = 35;
			break;
		case 3:
			y = 53;
			break;
	}
	
	if(KEY_C_sign)
	{
		switch(rad)
		{
			case 2:
				rad = 3;
				UI_ARM_sign = 1;
				break;
			case 3:
				rad = 2;
				UI_ARM_sign = 0;
				break;
		}
		KEY_C_sign = 0;
	}
	u8g2_DrawDisc(&u8g2,x,y,rad,U8G2_DRAW_ALL);
	u8g2_SendBuffer(&u8g2);
}

void page_1_circle_change_speed()
{
	static uint8_t x = 0;
	switch(change_speed_sign)
	{
		case 0:
			x = 31;
			break;
		case 1:
			x = 22;
			break;
		case 2:
			x = 13;
			break;
		case 3:
			x = 4;
			break;
	}
	u8g2_DrawCircle(&u8g2,4,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,13,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,22,4,3,U8G2_DRAW_ALL);
	u8g2_DrawCircle(&u8g2,31,4,3,U8G2_DRAW_ALL);
	u8g2_DrawDisc(&u8g2,x,4,3,U8G2_DRAW_ALL);
	u8g2_SendBuffer(&u8g2);
}

void page_1_update()
{
	if(option_change_sign)
	{
		u8g2_ClearBuffer(&u8g2);
		page_1_init();
		page_1_circle_num();
		page_1_circle_change_speed();
		option_change_sign = 0;
	}
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%4d",PWM_JXB_1);
	u8g2_DrawStr(&u8g2,24,23,(char*)str_buff);  
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%4d",PWM_JXB_2);
	u8g2_DrawStr(&u8g2,24,41,(char*)str_buff);   
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%4d",PWM_JXB_3);
	u8g2_DrawStr(&u8g2,24,59,(char*)str_buff);
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%4d",PWM_JXB_4);
	u8g2_DrawStr(&u8g2,87,23,(char*)str_buff);
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%4d",PWM_JXB_5);
	u8g2_DrawStr(&u8g2,87,41,(char*)str_buff);
	
	u8g2_SetFont(&u8g2, u8g2_font_8x13_mf);
	sprintf((char*)str_buff,"%4d",PWM_JXB_6);
	u8g2_DrawStr(&u8g2,87,59,(char*)str_buff);
	
	u8g2_SendBuffer(&u8g2);
}

void ARM_Control(volatile uint32_t* num , uint8_t up)
{
		if(up)
		{
			switch(change_speed_sign)
			{
				case 0:
					if(*num+1 <= 2500)	*num +=1;
					else								*num = 2500;
					break;
				case 1:
					if(*num+10 <= 2500)	*num +=10;
					else								*num = 2500;
					break;
				case 2:
					if(*num+100 <= 2500)	*num +=100;
					else									*num = 2500;
					break;
				case 3:
					if(*num+1000 <= 2500)	*num +=1000;
					else									*num = 2500;
					break;
			}
		}
		else
		{
			switch(change_speed_sign)
			{
				case 0:
					if(*num-1 >= 100 && *num-1 <= 2500)	*num -=1;
					else							*num = 100;
					break;
				case 1:
					if(*num-10 >= 100 && *num-10 <= 2500)	*num -=10;
					else								*num = 100;
					break;
				case 2:
					if(*num-100 >= 100 && *num-100 <= 2500)	*num -=100;
					else								*num = 100;
					break;
				case 3:
					if(*num-1000 >= 100 && *num-1000 <= 2500)	*num -=1000;
					else									*num = 100;
					break;
			}
		}
}

void ARM_Control_ALL(uint8_t option[] , uint8_t up)
{
	if(option[0]==1)
	{
		switch(option[1])
		{
			case 1:
				ARM_Control(&PWM_JXB_1,up);
				break;
			case 2:
				ARM_Control(&PWM_JXB_2,up);
				break;
			case 3:
				ARM_Control(&PWM_JXB_3,up);
				break;
		}
	}
	else
	{
		switch(option[1])
		{
			case 1:
				ARM_Control(&PWM_JXB_4,up);
				break;
			case 2:
				ARM_Control(&PWM_JXB_5,up);
				break;
			case 3:
				ARM_Control(&PWM_JXB_6,up);
				break;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY_L_PIN)
	{
		if(KEY_L)
		{
			if(UI_ARM_sign)
			{
				if(change_speed_sign==3)
				{
					change_speed_sign = 0;
				}
				else
				{
					change_speed_sign++;
				}
				page_1_circle_change_speed();
			}
			else
			{
				switch(option[0])
				{
					case 1:
						option[0] = 2;
						break;
					case 2:
						option[0] = 1;
						break;
				}
			}
			option_change_sign = 1;
		}
	}
	
	if(GPIO_Pin==KEY_R_PIN)
	{
		if(KEY_R)
		{
			if(UI_ARM_sign)
			{
				if(change_speed_sign==0)
				{
					change_speed_sign = 3;
				}
				else
				{
					change_speed_sign--;
				}
				page_1_circle_change_speed();
			}
			else
			{
				switch(option[0])
				{
					case 1:
						option[0] = 2;
						break;
					case 2:
						option[0] = 1;
						break;
				}
			}
			option_change_sign = 1;
		}
	}
	
	if(GPIO_Pin==KEY_T_PIN)
	{
		if(KEY_T)
		{
			if(UI_ARM_sign)
			{
				ARM_Control_ALL(option,1);
			}
			else
			{
				switch(option[1])
				{
					case 1:
						option[1] = 3;
						break;
					case 2:
						option[1] = 1;
						break;
					case 3:
						option[1] = 2;
						break;
				}
			}
			option_change_sign = 1;
		}
	}
	
	if(GPIO_Pin==KEY_B_PIN)
	{
		if(KEY_B)
		{
			if(UI_ARM_sign)
			{
				ARM_Control_ALL(option,0);
			}
			else
			{
				switch(option[1])
				{
					case 1:
						option[1] = 2;
						break;
					case 2:
						option[1] = 3;
						break;
					case 3:
						option[1] = 1;
						break;
				}
			}
			option_change_sign = 1;
		}
	}
	
	if(GPIO_Pin==KEY_C_PIN)
	{
		if(KEY_C)
		{
			KEY_C_sign = 1;
			option_change_sign = 1;
		}
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	///////////////////////////////OLED显示/////////////////////////////
	u8g2_Setup_sh1106_128x64_noname_f(&u8g2,U8G2_R0,u8x8_byte_4wire_hw_spi,u8g2_gpio_and_delay_stm32);
	u8g2_InitDisplay(&u8g2); 
	u8g2_SetPowerSave(&u8g2,0);
	/////////////////////////////////////////////   TIM1_DMA   /////////////////////	
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);		
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	/////////////////////////////////////////////   TIM2_DMA   /////////////////////	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);		
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	/////////////////////////////////////////////   TIM3_DMA   /////////////////////	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);	
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
//	/////////////////////////////////////////////   TIM4   /////////////////////////
//	HAL_TIM_Base_Start(&htim4);
	/////////////////////////////////////////////   ARM   /////////////////////////
	PWM_JXB_1 = 1900; //500-2500
	PWM_JXB_2 = 1200; //500-2500
	PWM_JXB_3 = 500; //500-2500
	PWM_JXB_4 = 1500; //500-2500
	PWM_JXB_5 = 1000; //500-2500
	PWM_JXB_6 = 1800; //500-2500
	/////////////////////////////////////////////   UI   /////////////////////////
	page_1_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		page_1_update();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
