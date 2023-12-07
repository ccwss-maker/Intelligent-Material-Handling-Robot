#ifndef Motro_Control_Wheel_H
#define Motro_Control_Wheel_H

#include "main.h"

typedef struct{
    uint16_t feedback_error;
    bool feedback_sign;
}Motro_Control_Wheel_;

void Motro_Transmit_Verify_Wheel(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint8_t* pVerify);

extern Motro_Control_Wheel_ Motro_Control_Wheel;

#endif
