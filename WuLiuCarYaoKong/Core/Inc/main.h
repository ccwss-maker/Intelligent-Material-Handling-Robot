/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define R_X_Pin GPIO_PIN_0
#define R_X_GPIO_Port GPIOA
#define R_Y_Pin GPIO_PIN_1
#define R_Y_GPIO_Port GPIOA
#define R3_Pin GPIO_PIN_2
#define R3_GPIO_Port GPIOA
#define L2_Pin GPIO_PIN_3
#define L2_GPIO_Port GPIOA
#define L1_Pin GPIO_PIN_4
#define L1_GPIO_Port GPIOA
#define L_X_Pin GPIO_PIN_5
#define L_X_GPIO_Port GPIOA
#define L_Y_Pin GPIO_PIN_6
#define L_Y_GPIO_Port GPIOA
#define L3_Pin GPIO_PIN_7
#define L3_GPIO_Port GPIOA
#define R2_Pin GPIO_PIN_0
#define R2_GPIO_Port GPIOB
#define R1_Pin GPIO_PIN_1
#define R1_GPIO_Port GPIOB
#define Up_Pin GPIO_PIN_2
#define Up_GPIO_Port GPIOB
#define Down_Pin GPIO_PIN_10
#define Down_GPIO_Port GPIOB
#define Left_Pin GPIO_PIN_11
#define Left_GPIO_Port GPIOB
#define Right_Pin GPIO_PIN_12
#define Right_GPIO_Port GPIOB
#define A_Pin GPIO_PIN_13
#define A_GPIO_Port GPIOB
#define Y_Pin GPIO_PIN_14
#define Y_GPIO_Port GPIOB
#define X_Pin GPIO_PIN_15
#define X_GPIO_Port GPIOB
#define B_Pin GPIO_PIN_8
#define B_GPIO_Port GPIOA
#define TX_Pin GPIO_PIN_6
#define TX_GPIO_Port GPIOB
#define RX_Pin GPIO_PIN_7
#define RX_GPIO_Port GPIOB
#define State_Pin GPIO_PIN_8
#define State_GPIO_Port GPIOB
#define State_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
