#ifndef wheel_h
#define wheel_h
#include <main.h>

#define Wheel_Addr_All 0
#define Wheel_Addr_FL 1
#define Wheel_Addr_FR 2
#define Wheel_Addr_BL 3
#define Wheel_Addr_BR 4

#define Car_a 0.1315
#define Car_b	0.084
#define Wheel_r 0.046

typedef struct{
    float W;
    float Yaw;
}PID_W_;

typedef struct {
    float x;
    float y;
    float w;
    float acc;
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
    PID_W_ PID_W;
    bool PID_Sign; //true:启用PID控制 false：禁用PID控制
    bool PID_Control_Sign;
    bool Close_sign;
    bool debug_sign;
}Wheel_;

void PID_Start();
void PID_Stop();
void Wheel_Init(void);
void Wheel_Open(void);
void Wheel_Close(void);
void Wheel_Speed_To_0(uint8_t acc);
void Wheel_Speed_Control(uint8_t Address, int16_t speed, uint8_t aacelerated);
void Wheel_Group_Speed_Control(float v_x, float v_y, float w, uint8_t aacelerated);
void Wheel_Closed_Loop_Init();
void Wheel_Closed_Loop_Spd_Set(float v_x, float v_y, float w, uint8_t aacelerated);

void PID_Calculate_Control();
float PID_Speed_Calculate(float Speed);
float PID_Position_Calculate(float Angle);

extern Wheel_ Wheel;
#endif
