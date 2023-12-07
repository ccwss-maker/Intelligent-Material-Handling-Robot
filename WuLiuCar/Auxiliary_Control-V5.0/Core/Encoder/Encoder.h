#ifndef Encoder_H
#define Encoder_H
#include <main.h>

#define CS1_H HAL_GPIO_WritePin(ENCODER_SPI1_CS1_GPIO_Port, ENCODER_SPI1_CS1_Pin, GPIO_PIN_SET)
#define CS1_L HAL_GPIO_WritePin(ENCODER_SPI1_CS1_GPIO_Port, ENCODER_SPI1_CS1_Pin, GPIO_PIN_RESET)
#define CS2_H HAL_GPIO_WritePin(ENCODER_SPI1_CS2_GPIO_Port, ENCODER_SPI1_CS2_Pin, GPIO_PIN_SET)
#define CS2_L HAL_GPIO_WritePin(ENCODER_SPI1_CS2_GPIO_Port, ENCODER_SPI1_CS2_Pin, GPIO_PIN_RESET)
#define CS3_H HAL_GPIO_WritePin(ENCODER_SPI1_CS3_GPIO_Port, ENCODER_SPI1_CS3_Pin, GPIO_PIN_SET)
#define CS3_L HAL_GPIO_WritePin(ENCODER_SPI1_CS3_GPIO_Port, ENCODER_SPI1_CS3_Pin, GPIO_PIN_RESET)

typedef struct{
	float angle;
	bool Over_Speed_Flag;
	bool Weak_Magnetic_Flag;
	bool Low_Voltage_Flag;
}Encoder_Data_;

void Encoder_Detection();

#endif
