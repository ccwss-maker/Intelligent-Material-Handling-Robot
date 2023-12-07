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
#include "stm32h7xx_hal.h"

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
#define TRACKING_I2C1_SCL_Pin GPIO_PIN_8
#define TRACKING_I2C1_SCL_GPIO_Port GPIOB
#define ENCODER_TIM4_CH1_Pin GPIO_PIN_6
#define ENCODER_TIM4_CH1_GPIO_Port GPIOB
#define STANDBY_UART7_RX_Pin GPIO_PIN_3
#define STANDBY_UART7_RX_GPIO_Port GPIOB
#define TF_SDMMC2_D2_Pin GPIO_PIN_11
#define TF_SDMMC2_D2_GPIO_Port GPIOG
#define TF_SDMMC2_D0_Pin GPIO_PIN_9
#define TF_SDMMC2_D0_GPIO_Port GPIOG
#define BLE_USART6_STATE_Pin GPIO_PIN_3
#define BLE_USART6_STATE_GPIO_Port GPIOD
#define SDRAM_FMC_D3_Pin GPIO_PIN_1
#define SDRAM_FMC_D3_GPIO_Port GPIOD
#define STANDBY_UART7_TX_Pin GPIO_PIN_15
#define STANDBY_UART7_TX_GPIO_Port GPIOA
#define ENCODER_SPI1_CS3_Pin GPIO_PIN_4
#define ENCODER_SPI1_CS3_GPIO_Port GPIOE
#define CAM_USART10_TX_Pin GPIO_PIN_3
#define CAM_USART10_TX_GPIO_Port GPIOE
#define TRACKING_I2C1_SDA_Pin GPIO_PIN_9
#define TRACKING_I2C1_SDA_GPIO_Port GPIOB
#define ENCODER_TIM4_CH2_Pin GPIO_PIN_7
#define ENCODER_TIM4_CH2_GPIO_Port GPIOB
#define ENCODER_TIM3_CH1_Pin GPIO_PIN_4
#define ENCODER_TIM3_CH1_GPIO_Port GPIOB
#define PWM_TIM23_CH2_Pin GPIO_PIN_13
#define PWM_TIM23_CH2_GPIO_Port GPIOG
#define TF_SDMMC2_CMD_Pin GPIO_PIN_7
#define TF_SDMMC2_CMD_GPIO_Port GPIOD
#define SDRAM_FMC_D6_Pin GPIO_PIN_12
#define SDRAM_FMC_D6_GPIO_Port GPIOC
#define TOUCH_I2C3_SCL_Pin GPIO_PIN_8
#define TOUCH_I2C3_SCL_GPIO_Port GPIOA
#define TOUCH_I2C3_INT_Pin GPIO_PIN_12
#define TOUCH_I2C3_INT_GPIO_Port GPIOA
#define TOUCH_I2C3_INT_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_SPI1_CS2_Pin GPIO_PIN_13
#define ENCODER_SPI1_CS2_GPIO_Port GPIOC
#define CAM_USART10_RX_Pin GPIO_PIN_2
#define CAM_USART10_RX_GPIO_Port GPIOE
#define ENCODER_TIM3_CH2_Pin GPIO_PIN_5
#define ENCODER_TIM3_CH2_GPIO_Port GPIOB
#define TF_SDMMC2_INT_Pin GPIO_PIN_14
#define TF_SDMMC2_INT_GPIO_Port GPIOG
#define TF_SDMMC2_D1_Pin GPIO_PIN_10
#define TF_SDMMC2_D1_GPIO_Port GPIOG
#define SDRAM_FMC_D2_Pin GPIO_PIN_0
#define SDRAM_FMC_D2_GPIO_Port GPIOD
#define STANDBY_UART4_RX_Pin GPIO_PIN_11
#define STANDBY_UART4_RX_GPIO_Port GPIOC
#define STANDBY_UART4_TX_Pin GPIO_PIN_10
#define STANDBY_UART4_TX_GPIO_Port GPIOC
#define WHEEL_USART1_RX_Pin GPIO_PIN_10
#define WHEEL_USART1_RX_GPIO_Port GPIOA
#define TOUCH_I2C3_RST_Pin GPIO_PIN_11
#define TOUCH_I2C3_RST_GPIO_Port GPIOA
#define ENCODER_SPI1_CS1_Pin GPIO_PIN_15
#define ENCODER_SPI1_CS1_GPIO_Port GPIOC
#define SDRAM_FMC_SDNCAS_Pin GPIO_PIN_15
#define SDRAM_FMC_SDNCAS_GPIO_Port GPIOG
#define TF_SDMMC2_D3_Pin GPIO_PIN_12
#define TF_SDMMC2_D3_GPIO_Port GPIOG
#define TF_SDMMC2_CK_Pin GPIO_PIN_6
#define TF_SDMMC2_CK_GPIO_Port GPIOD
#define ENCODER_TIM1_CH2_Pin GPIO_PIN_9
#define ENCODER_TIM1_CH2_GPIO_Port GPIOA
#define BLE_USART6_RX_Pin GPIO_PIN_7
#define BLE_USART6_RX_GPIO_Port GPIOC
#define TOUCH_I2C3_SDA_Pin GPIO_PIN_9
#define TOUCH_I2C3_SDA_GPIO_Port GPIOC
#define BLE_USART6_TX_Pin GPIO_PIN_6
#define BLE_USART6_TX_GPIO_Port GPIOC
#define SDRAM_FMC_A1_Pin GPIO_PIN_1
#define SDRAM_FMC_A1_GPIO_Port GPIOF
#define SDRAM_FMC_A0_Pin GPIO_PIN_0
#define SDRAM_FMC_A0_GPIO_Port GPIOF
#define FLASH_QSPI_NCS_Pin GPIO_PIN_6
#define FLASH_QSPI_NCS_GPIO_Port GPIOG
#define SDRAM_FMC_BA1_Pin GPIO_PIN_5
#define SDRAM_FMC_BA1_GPIO_Port GPIOG
#define SDRAM_FMC_A2_Pin GPIO_PIN_2
#define SDRAM_FMC_A2_GPIO_Port GPIOF
#define SDRAM_FMC_SDCLK_Pin GPIO_PIN_8
#define SDRAM_FMC_SDCLK_GPIO_Port GPIOG
#define SDRAM_FMC_BA0_Pin GPIO_PIN_4
#define SDRAM_FMC_BA0_GPIO_Port GPIOG
#define SDRAM_FMC_A12_Pin GPIO_PIN_2
#define SDRAM_FMC_A12_GPIO_Port GPIOG
#define FLASH_QSPI_IO3_Pin GPIO_PIN_6
#define FLASH_QSPI_IO3_GPIO_Port GPIOF
#define SDRAM_FMC_A4_Pin GPIO_PIN_4
#define SDRAM_FMC_A4_GPIO_Port GPIOF
#define SDRAM_FMC_A5_Pin GPIO_PIN_5
#define SDRAM_FMC_A5_GPIO_Port GPIOF
#define SDRAM_FMC_A3_Pin GPIO_PIN_3
#define SDRAM_FMC_A3_GPIO_Port GPIOF
#define SDRAM_FMC_D0_Pin GPIO_PIN_14
#define SDRAM_FMC_D0_GPIO_Port GPIOD
#define FLASH_QSPI_IO0_Pin GPIO_PIN_8
#define FLASH_QSPI_IO0_GPIO_Port GPIOF
#define FLASH_QSPI_IO1_Pin GPIO_PIN_9
#define FLASH_QSPI_IO1_GPIO_Port GPIOF
#define SDRAM_FMC_D1_Pin GPIO_PIN_15
#define SDRAM_FMC_D1_GPIO_Port GPIOD
#define FLASH_QSPI_CLK_Pin GPIO_PIN_10
#define FLASH_QSPI_CLK_GPIO_Port GPIOF
#define SDRAM_FMC_D14_Pin GPIO_PIN_9
#define SDRAM_FMC_D14_GPIO_Port GPIOD
#define OLED_SPI2_MOSI_Pin GPIO_PIN_15
#define OLED_SPI2_MOSI_GPIO_Port GPIOB
#define WHEEL_USART1_TX_Pin GPIO_PIN_14
#define WHEEL_USART1_TX_GPIO_Port GPIOB
#define SDRAM_FMC_SDNWE_Pin GPIO_PIN_0
#define SDRAM_FMC_SDNWE_GPIO_Port GPIOC
#define SDRAM_FMC_D15_Pin GPIO_PIN_10
#define SDRAM_FMC_D15_GPIO_Port GPIOD
#define SDRAM_FMC_D13_Pin GPIO_PIN_8
#define SDRAM_FMC_D13_GPIO_Port GPIOD
#define OLED_SPI2_SCK_Pin GPIO_PIN_13
#define OLED_SPI2_SCK_GPIO_Port GPIOB
#define FLASH_QSPI_IO2_Pin GPIO_PIN_2
#define FLASH_QSPI_IO2_GPIO_Port GPIOC
#define SDRAM_FMC_SDCKE0_Pin GPIO_PIN_5
#define SDRAM_FMC_SDCKE0_GPIO_Port GPIOC
#define OLED_SPI2_CS_Pin GPIO_PIN_12
#define OLED_SPI2_CS_GPIO_Port GPIOB
#define ENCODER_SPI1_MOSI_Pin GPIO_PIN_7
#define ENCODER_SPI1_MOSI_GPIO_Port GPIOA
#define SDRAM_FMC_SDNRAS_Pin GPIO_PIN_11
#define SDRAM_FMC_SDNRAS_GPIO_Port GPIOF
#define SDRAM_FMC_D5_Pin GPIO_PIN_8
#define SDRAM_FMC_D5_GPIO_Port GPIOE
#define SDRAM_FMC_A11_Pin GPIO_PIN_1
#define SDRAM_FMC_A11_GPIO_Port GPIOG
#define SDRAM_FMC_A9_Pin GPIO_PIN_15
#define SDRAM_FMC_A9_GPIO_Port GPIOF
#define SDRAM_FMC_A7_Pin GPIO_PIN_13
#define SDRAM_FMC_A7_GPIO_Port GPIOF
#define ARM_USART3_TX_Pin GPIO_PIN_10
#define ARM_USART3_TX_GPIO_Port GPIOB
#define OLED_SPI2_DC_Pin GPIO_PIN_12
#define OLED_SPI2_DC_GPIO_Port GPIOH
#define ENCODER_TIM2_CH1_Pin GPIO_PIN_0
#define ENCODER_TIM2_CH1_GPIO_Port GPIOA
#define ENCODER_TIM2_CH2_Pin GPIO_PIN_1
#define ENCODER_TIM2_CH2_GPIO_Port GPIOA
#define IMU_I2C2_SCL_Pin GPIO_PIN_4
#define IMU_I2C2_SCL_GPIO_Port GPIOH
#define ENCODER_SPI1_SCK_Pin GPIO_PIN_5
#define ENCODER_SPI1_SCK_GPIO_Port GPIOA
#define SDRAM_FMC_A10_Pin GPIO_PIN_0
#define SDRAM_FMC_A10_GPIO_Port GPIOG
#define SDRAM_FMC_D4_Pin GPIO_PIN_7
#define SDRAM_FMC_D4_GPIO_Port GPIOE
#define ARM_USART3_RX_Pin GPIO_PIN_11
#define ARM_USART3_RX_GPIO_Port GPIOB
#define SDRAM_FMC_A6_Pin GPIO_PIN_12
#define SDRAM_FMC_A6_GPIO_Port GPIOF
#define SDRAM_FMC_D9_Pin GPIO_PIN_12
#define SDRAM_FMC_D9_GPIO_Port GPIOE
#define SDRAM_FMC_D10_Pin GPIO_PIN_13
#define SDRAM_FMC_D10_GPIO_Port GPIOE
#define SDRAM_FMC_D12_Pin GPIO_PIN_15
#define SDRAM_FMC_D12_GPIO_Port GPIOE
#define OLED_SPI2_RES_Pin GPIO_PIN_6
#define OLED_SPI2_RES_GPIO_Port GPIOH
#define Battery_ADC1_INP0_Pin GPIO_PIN_0
#define Battery_ADC1_INP0_GPIO_Port GPIOA
#define SDRAM_FMC_SDNE0_Pin GPIO_PIN_3
#define SDRAM_FMC_SDNE0_GPIO_Port GPIOH
#define IMU_I2C2_SDA_Pin GPIO_PIN_5
#define IMU_I2C2_SDA_GPIO_Port GPIOH
#define ENCODER_SPI1_MISO_Pin GPIO_PIN_6
#define ENCODER_SPI1_MISO_GPIO_Port GPIOA
#define SDRAM_FMC_A8_Pin GPIO_PIN_14
#define SDRAM_FMC_A8_GPIO_Port GPIOF
#define ENCODER_TIM1_CH1_Pin GPIO_PIN_9
#define ENCODER_TIM1_CH1_GPIO_Port GPIOE
#define SDRAM_FMC_D8_Pin GPIO_PIN_11
#define SDRAM_FMC_D8_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
