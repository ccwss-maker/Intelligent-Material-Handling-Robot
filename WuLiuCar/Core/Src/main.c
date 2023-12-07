/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <ST7789V.h>
#include <GT911.h>
#include "stdio.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
#include "lv_app.h"
#include "imu.h"
#include "ble.h"
#include "wheel.h"
#include "Arm.h"
#include "Tracking.h"
#include "Battery.h"
#include "Motro_Control.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == TOUCH_INT_Pin)
  {
		gt911_Scanf();
	}
  else if(GPIO_Pin == BLE_STATE_Pin)
  {
    BLE_State_Display();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3)
	{
		lv_tick_inc(1);
	}
	else if(htim == &htim4)
	{
    Motro_UART_Timeout();
	}
  else if(htim == &htim5)
  {
    BLE_Arm.Control_Sign = true;
  }
	else if(htim == &htim6)
	{
    Tracking_Transmit();
		if(Arm.debug_sign)
    {
			//Arm_Angle_Check();
			//Arm_Encoder_Check();
    }
	}
	else if(htim == &htim14)
	{
		Battery.Tese_Sign = true;
	}

  Motor_Position_TIM_IT(htim);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if(hi2c == &hi2c3)
  {
    Tracking_Receive();
  }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) 
{
  if(hi2c == &hi2c3)
  {
    Tracking_ErrorCallback(hi2c);
  }
}

// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
// {
//   if(huart == &huart1)
//   {
//     // Motro_UART_Transmit_IT(); 
//   }
// }

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//   if(huart == &huart1)
//   {
//     // Motro_UART_Receive_IT(); 
//   }
// }
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM10_Init();
  MX_TIM3_Init();
  MX_TIM14_Init();
  MX_ADC1_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_TIM6_Init();
  MX_TIM4_Init();
  MX_TIM13_Init();
  MX_TIM7_Init();
  MX_TIM9_Init();
  MX_TIM11_Init();
  MX_TIM12_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
	///////////////////////////////////   LVGL时钟   /////////////////////////////////
	HAL_TIM_Base_Start_IT(&htim3);
  ///////////////////////////////////   机械臂计算初始化   ///////////////////////////
  Arm_Angle_Calculate_Init();
  /////////////////////////////////    LVGL_INIT  ///////////////////////////////
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
  lv_fs_fatfs_init();
	lv_demo_myself();
  ///////////////////////////////////   电量棿测时钿   /////////////////////////////////
	HAL_ADC_Start(&hadc1);
	HAL_TIM_Base_Start_IT(&htim14);
  //////////////////////////////////   Motro_Control_Init   ///////////////////////////////
  HAL_Delay(1000);
  Motro_Control_Init();
  __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE); /*清楚中断标志，防止直接进入中�??????*/
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1, Motro_Control.UART.receive, Motro_UART_Buffer);
  //////////////////////////////////   ARM_Init   ///////////////////////////////
  __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
  __HAL_TIM_CLEAR_IT(&htim9, TIM_IT_UPDATE);
  __HAL_TIM_CLEAR_IT(&htim11, TIM_IT_UPDATE);
  Arm_Init();
  ////////////////////////////////   Wheel_Init   ///////////////////////////////
  Wheel_Init();
  ////////////////////////////////////////////    IMU_Init ///////////////////////
  IMU_Init();
	HAL_UART_Receive_DMA(&huart2,(uint8_t*)imu_uart.receive,imu_uart_buffer);
  //////////////////////////////////////////////    BLE_Init ///////////////////////
  BLE_Init();
  __HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE); 
	HAL_UART_Receive_DMA(&huart6,(uint8_t*)ble_uart.receive,ble_uart_buffer);
  ///////////////////////////////   循迹模块时钟   /////////////////////////////////
  __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE);
  HAL_TIM_Base_Start_IT(&htim6);
  // //////////////////////////////////   PID频率�????�????   ///////////////////////////////
  // __HAL_TIM_CLEAR_IT(&htim13, TIM_IT_UPDATE);
  // HAL_TIM_Base_Start(&htim13);
  //////////////////////////////////   蓝牙控制时钟   ///////////////////////////////
  __HAL_TIM_CLEAR_IT(&htim5, TIM_IT_UPDATE);
  //////////////////////////////////       PWM       ///////////////////////////////
  HAL_TIM_PWM_Start(&htim10,TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
		lv_task_handler();
    BLE_control();
    BLE_Arm_Control();
    IMU_Charting();
    PID_Charting();
    Tracking_Draw();
		Battery_Test();
    Motro_Control_Text();
    Arm_Symbol_Refresh();
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
