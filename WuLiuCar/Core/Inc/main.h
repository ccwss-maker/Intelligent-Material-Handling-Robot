/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define PRINTF(title, fmt, args...)  printf("{"#title"}"fmt"\n", ##args)
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BLE_State_Pin GPIO_PIN_13
#define BLE_State_GPIO_Port GPIOC
#define IMU_TX_Pin GPIO_PIN_2
#define IMU_TX_GPIO_Port GPIOA
#define IMU_RX_Pin GPIO_PIN_3
#define IMU_RX_GPIO_Port GPIOA
#define KEY_TL_Pin GPIO_PIN_4
#define KEY_TL_GPIO_Port GPIOA
#define KEY_L_Pin GPIO_PIN_5
#define KEY_L_GPIO_Port GPIOA
#define KEY_BL_Pin GPIO_PIN_6
#define KEY_BL_GPIO_Port GPIOA
#define KEY_T_Pin GPIO_PIN_7
#define KEY_T_GPIO_Port GPIOA
#define KEY_C_Pin GPIO_PIN_4
#define KEY_C_GPIO_Port GPIOC
#define KEY_B_Pin GPIO_PIN_5
#define KEY_B_GPIO_Port GPIOC
#define KEY_TR_Pin GPIO_PIN_0
#define KEY_TR_GPIO_Port GPIOB
#define KEY_R_Pin GPIO_PIN_1
#define KEY_R_GPIO_Port GPIOB
#define KEY_BR_Pin GPIO_PIN_2
#define KEY_BR_GPIO_Port GPIOB
#define BLE_TX_Pin GPIO_PIN_6
#define BLE_TX_GPIO_Port GPIOC
#define BLE_RX_Pin GPIO_PIN_7
#define BLE_RX_GPIO_Port GPIOC
#define Tracking_SDA_Pin GPIO_PIN_9
#define Tracking_SDA_GPIO_Port GPIOC
#define Tracking_SCL_Pin GPIO_PIN_8
#define Tracking_SCL_GPIO_Port GPIOA
#define Motro_TX_Pin GPIO_PIN_9
#define Motro_TX_GPIO_Port GPIOA
#define Motro_RX_Pin GPIO_PIN_10
#define Motro_RX_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_15
#define OLED_DC_GPIO_Port GPIOA
#define OLED_SCK_Pin GPIO_PIN_10
#define OLED_SCK_GPIO_Port GPIOC
#define OLED_RES_Pin GPIO_PIN_11
#define OLED_RES_GPIO_Port GPIOC
#define OLED_MOSI_Pin GPIO_PIN_12
#define OLED_MOSI_GPIO_Port GPIOC
#define PWM_Arm_1_Pin GPIO_PIN_4
#define PWM_Arm_1_GPIO_Port GPIOB
#define PWM_Arm_2_Pin GPIO_PIN_5
#define PWM_Arm_2_GPIO_Port GPIOB
#define PWM_Arm_3_Pin GPIO_PIN_6
#define PWM_Arm_3_GPIO_Port GPIOB
#define PWM_Arm_4_Pin GPIO_PIN_7
#define PWM_Arm_4_GPIO_Port GPIOB
#define PWM_Arm_5_Pin GPIO_PIN_8
#define PWM_Arm_5_GPIO_Port GPIOB
#define PWM_Arm_6_Pin GPIO_PIN_9
#define PWM_Arm_6_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define	delay_3 10
#define delay_2 60	
#define delay_1 100		
#define PWM_JXB_1 TIM3->CCR1
#define PWM_JXB_2 TIM3->CCR2
#define PWM_JXB_3 TIM4->CCR1
#define PWM_JXB_4 TIM4->CCR2
#define PWM_JXB_5 TIM4->CCR3
#define PWM_JXB_6 TIM4->CCR4
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
