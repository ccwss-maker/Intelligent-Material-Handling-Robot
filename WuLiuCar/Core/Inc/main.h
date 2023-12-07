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
#define TOUCH_INT_Pin GPIO_PIN_14
#define TOUCH_INT_GPIO_Port GPIOC
#define TOUCH_INT_EXTI_IRQn EXTI15_10_IRQn
#define TOUCH_RST_Pin GPIO_PIN_15
#define TOUCH_RST_GPIO_Port GPIOC
#define UART_IMU_TX_MCU_Pin GPIO_PIN_2
#define UART_IMU_TX_MCU_GPIO_Port GPIOA
#define UART_IMU_RX_MCU_Pin GPIO_PIN_3
#define UART_IMU_RX_MCU_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_4
#define OLED_RES_GPIO_Port GPIOA
#define OLED_SCK_Pin GPIO_PIN_5
#define OLED_SCK_GPIO_Port GPIOA
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOA
#define OLED_MOSI_Pin GPIO_PIN_7
#define OLED_MOSI_GPIO_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_4
#define OLED_CS_GPIO_Port GPIOC
#define UART_CAM_RX_Pin GPIO_PIN_5
#define UART_CAM_RX_GPIO_Port GPIOC
#define I2C_NVIDIA_SCL_Pin GPIO_PIN_10
#define I2C_NVIDIA_SCL_GPIO_Port GPIOB
#define UART_BLE_TX_MCU_Pin GPIO_PIN_6
#define UART_BLE_TX_MCU_GPIO_Port GPIOC
#define UART_BLE_RX_MCU_Pin GPIO_PIN_7
#define UART_BLE_RX_MCU_GPIO_Port GPIOC
#define I2C_TRACKING_SCL_Pin GPIO_PIN_8
#define I2C_TRACKING_SCL_GPIO_Port GPIOA
#define SDIO_INT_Pin GPIO_PIN_10
#define SDIO_INT_GPIO_Port GPIOA
#define UART_MOTRO_TX_Pin GPIO_PIN_15
#define UART_MOTRO_TX_GPIO_Port GPIOA
#define UART_CAM_TX_Pin GPIO_PIN_10
#define UART_CAM_TX_GPIO_Port GPIOC
#define UART_MOTRO_RX_Pin GPIO_PIN_3
#define UART_MOTRO_RX_GPIO_Port GPIOB
#define I2C_TRACKING_SDA_Pin GPIO_PIN_4
#define I2C_TRACKING_SDA_GPIO_Port GPIOB
#define BLE_STATE_Pin GPIO_PIN_5
#define BLE_STATE_GPIO_Port GPIOB
#define BLE_STATE_EXTI_IRQn EXTI9_5_IRQn
#define I2C_TOUCH_SCL_Pin GPIO_PIN_6
#define I2C_TOUCH_SCL_GPIO_Port GPIOB
#define I2C_TOUCH_SDA_Pin GPIO_PIN_7
#define I2C_TOUCH_SDA_GPIO_Port GPIOB
#define PWM_ARM_Pin GPIO_PIN_8
#define PWM_ARM_GPIO_Port GPIOB
#define I2C_NVIDIA_SDA_Pin GPIO_PIN_9
#define I2C_NVIDIA_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
