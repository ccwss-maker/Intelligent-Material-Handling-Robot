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
#define IN_1_Pin GPIO_PIN_13
#define IN_1_GPIO_Port GPIOC
#define IN_1_EXTI_IRQn EXTI15_10_IRQn
#define PWM_JXB_1_Pin GPIO_PIN_0
#define PWM_JXB_1_GPIO_Port GPIOA
#define PWM_JXB_2_Pin GPIO_PIN_1
#define PWM_JXB_2_GPIO_Port GPIOA
#define PWM_JXB_3_Pin GPIO_PIN_2
#define PWM_JXB_3_GPIO_Port GPIOA
#define PWM_JXB_6_Pin GPIO_PIN_3
#define PWM_JXB_6_GPIO_Port GPIOA
#define IN_5_Pin GPIO_PIN_4
#define IN_5_GPIO_Port GPIOA
#define IN_5_EXTI_IRQn EXTI4_IRQn
#define BMQ_FL_B_Pin GPIO_PIN_5
#define BMQ_FL_B_GPIO_Port GPIOA
#define BMQ_FL_B_EXTI_IRQn EXTI9_5_IRQn
#define BMQ_FL_A_Pin GPIO_PIN_6
#define BMQ_FL_A_GPIO_Port GPIOA
#define BMQ_FL_A_EXTI_IRQn EXTI9_5_IRQn
#define BMQ_FR_B_Pin GPIO_PIN_7
#define BMQ_FR_B_GPIO_Port GPIOA
#define BMQ_FR_B_EXTI_IRQn EXTI9_5_IRQn
#define BMQ_FR_A_Pin GPIO_PIN_0
#define BMQ_FR_A_GPIO_Port GPIOB
#define BMQ_FR_A_EXTI_IRQn EXTI0_IRQn
#define BMQ_BL_B_Pin GPIO_PIN_1
#define BMQ_BL_B_GPIO_Port GPIOB
#define BMQ_BL_B_EXTI_IRQn EXTI1_IRQn
#define BMQ_BL_A_Pin GPIO_PIN_2
#define BMQ_BL_A_GPIO_Port GPIOB
#define BMQ_BL_A_EXTI_IRQn EXTI2_IRQn
#define BMQ_BR_B_Pin GPIO_PIN_10
#define BMQ_BR_B_GPIO_Port GPIOB
#define BMQ_BR_B_EXTI_IRQn EXTI15_10_IRQn
#define BMQ_BR_A_Pin GPIO_PIN_11
#define BMQ_BR_A_GPIO_Port GPIOB
#define BMQ_BR_A_EXTI_IRQn EXTI15_10_IRQn
#define OLED_DC_Pin GPIO_PIN_12
#define OLED_DC_GPIO_Port GPIOB
#define OLED_SCK_Pin GPIO_PIN_13
#define OLED_SCK_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_14
#define OLED_RES_GPIO_Port GPIOB
#define OLED_MOSI_Pin GPIO_PIN_15
#define OLED_MOSI_GPIO_Port GPIOB
#define PWM_BR_Pin GPIO_PIN_8
#define PWM_BR_GPIO_Port GPIOA
#define PWM_BL_Pin GPIO_PIN_9
#define PWM_BL_GPIO_Port GPIOA
#define PWM_FR_Pin GPIO_PIN_10
#define PWM_FR_GPIO_Port GPIOA
#define PWM_FL_Pin GPIO_PIN_11
#define PWM_FL_GPIO_Port GPIOA
#define IN_4_Pin GPIO_PIN_12
#define IN_4_GPIO_Port GPIOA
#define IN_4_EXTI_IRQn EXTI15_10_IRQn
#define IN_3_Pin GPIO_PIN_15
#define IN_3_GPIO_Port GPIOA
#define IN_3_EXTI_IRQn EXTI15_10_IRQn
#define IN_2_Pin GPIO_PIN_3
#define IN_2_GPIO_Port GPIOB
#define IN_2_EXTI_IRQn EXTI3_IRQn
#define PWM_JXB_5_Pin GPIO_PIN_4
#define PWM_JXB_5_GPIO_Port GPIOB
#define PWM_JXB_4_Pin GPIO_PIN_5
#define PWM_JXB_4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
