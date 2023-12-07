#ifndef Motro_Control_Arm_H
#define Motro_Control_Arm_H

#include "main.h"
#include "Motro_Control_Queul.h"

typedef struct{
    uint16_t feedback_error;
    bool feedback_sign;
}Motro_Control_Arm_;

void Motro_Transmit_Verify_Arm(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint8_t* pVerify);
void Motor_Absolute_Control(uint8_t Address, uint16_t speed, uint8_t accelerated, float angle);

extern Motro_Control_Arm_ Motro_Control_Arm;

#endif
