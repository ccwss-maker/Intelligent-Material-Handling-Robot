#ifndef wheel_h
#define wheel_h
#include <main.h>

#define Car_a 0.1315
#define Car_b	0.084
#define Wheel_r 0.046
#define Wheel_UART_Buffer 5

typedef struct 
{
    float FL;
    float FR;
    float BL;
    float BR;
    float FL_last;
    float FR_last;
    float BL_last;
    float BR_last;
}SPD_;

typedef struct{
    SPD_ SPD;
    bool PID_Sign; //true:启用PID控制 false：禁用PID控制
    bool Close_sign;
    bool debug_sign;
}Wheel_;

void Wheel_Init(void);
void Wheel_Open(void);
void Wheel_Close(void);
void Wheel_Group_Speed_Control(float v_x, float v_y, float w, uint8_t aacelerated);
void Wheel_Group_Closed_Loop_Control(float v_x, float v_y, float w, uint8_t aacelerated);

void PID_Debug_Speed(float Speed);
void PID_Debug_Position(float Angle);

extern Wheel_ Wheel;
#endif
