/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define ENCODER_SPI1_SCK_Pin GPIO_PIN_5
#define ENCODER_SPI1_SCK_GPIO_Port GPIOA
#define ENCODER_SPI1_MISO_Pin GPIO_PIN_6
#define ENCODER_SPI1_MISO_GPIO_Port GPIOA
#define ENCODER_SPI1_MOSI_Pin GPIO_PIN_7
#define ENCODER_SPI1_MOSI_GPIO_Port GPIOA
#define ENCODER_SPI1_CS1_Pin GPIO_PIN_0
#define ENCODER_SPI1_CS1_GPIO_Port GPIOB
#define ENCODER_SPI1_CS2_Pin GPIO_PIN_1
#define ENCODER_SPI1_CS2_GPIO_Port GPIOB
#define ENCODER_SPI1_CS3_Pin GPIO_PIN_2
#define ENCODER_SPI1_CS3_GPIO_Port GPIOB
#define MASTER_SLAVE_SPI2_SCK_Pin GPIO_PIN_13
#define MASTER_SLAVE_SPI2_SCK_GPIO_Port GPIOB
#define MASTER_SLAVE_SPI2_MISO_Pin GPIO_PIN_14
#define MASTER_SLAVE_SPI2_MISO_GPIO_Port GPIOB
#define MASTER_SLAVE_SPI2_MOSI_Pin GPIO_PIN_15
#define MASTER_SLAVE_SPI2_MOSI_GPIO_Port GPIOB
#define BLE_UART1_STATE_Pin GPIO_PIN_11
#define BLE_UART1_STATE_GPIO_Port GPIOA
#define BLE_MCU_UART1_TX_Pin GPIO_PIN_6
#define BLE_MCU_UART1_TX_GPIO_Port GPIOB
#define BLE_MCU_UART1_RX_Pin GPIO_PIN_7
#define BLE_MCU_UART1_RX_GPIO_Port GPIOB
#define TRACKING_I2C1_SCL_Pin GPIO_PIN_8
#define TRACKING_I2C1_SCL_GPIO_Port GPIOB
#define TRACKING_I2C1_SDA_Pin GPIO_PIN_9
#define TRACKING_I2C1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
