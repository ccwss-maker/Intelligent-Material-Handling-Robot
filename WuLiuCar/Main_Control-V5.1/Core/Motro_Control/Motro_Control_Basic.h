#ifndef Motor_Control_Basic_H
#define Motor_Control_Basic_H

#include "main.h"
#include "usart.h"

enum{
    Clockwise = 1,
    Counterclockwise = -1,
};

#define UART_Wheel  &huart1
#define UART_Arm    &huart2

uint8_t XOR_Calculate(uint8_t data[] , uint8_t length);
void Motor_Open(UART_HandleTypeDef *huart, uint8_t Address);
void Motor_Close(UART_HandleTypeDef *huart, uint8_t Address);
void Motor_MStep_Set(UART_HandleTypeDef *huart, uint8_t Address, uint8_t num);  //设置细分步数
void Motor_Set_0(UART_HandleTypeDef *huart, uint8_t Address);
void Motor_Speed_Control(UART_HandleTypeDef *huart, uint8_t Address, int16_t speed, uint8_t aacelerated);/*1为正方向，-1为反方向*/
void Motor_Relative_Position_Stop(UART_HandleTypeDef *huart, uint8_t Address);
void Motor_Relative_Position_Control(UART_HandleTypeDef *huart, uint8_t Address, int8_t direction, uint16_t speed, uint8_t accelerated, float angle, uint16_t Subdivision_Step, float Reduction_Ratio);
void Motro_Control_Init();

#endif
