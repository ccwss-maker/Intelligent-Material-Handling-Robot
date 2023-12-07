#ifndef MOTRO_CONTROL_H
#define MOTRO_CONTROL_H

#include "main.h"
#include "Motro_Control_Queul.h"

#define Angle_Control_Num 3

enum{
    Clockwise = 1,
    Counterclockwise = -1,
};

typedef struct
{
    float Value[7];
    bool Done_sign[7];
}Information_Inquire_;

typedef struct
{
    int8_t direction;
    uint16_t speed;
    uint8_t accelerated;
    bool Angle_Check_Sign;
    float angle;
    float angle_last;
    uint16_t Subdivision_Step;
    float Reduction_Ratio;
}Value_Pub_;

typedef struct
{
    Value_Pub_ Value_Pub[7];
    bool Pub_sign[7];
    bool Done_sign[7];
    uint8_t Done_Num;
}Angle_Control_;

typedef struct{
    UART_ UART;
    Information_Inquire_ Angle;
    Information_Inquire_ Encoder_Angle;
    Angle_Control_ Angle_Control;
    float Reduction_Ratio[7];
    float Encoder_Angle_Correction[7];
    uint16_t feedback_error;
    bool feedback_sign;
    bool uart_finish_sign;
}Motro_Control_;

/*通讯控制*/
void Motro_UART_Queue(struct Queue* queue, bool sign);
void Motro_UART_Timeout();
void Motro_IDLE_IT(uint8_t rx_len);
/*电机基础控制*/
void Motor_Open(uint8_t Address);
void Motor_Close(uint8_t Address);
void Motor_MStep_Set(uint8_t Address, uint8_t num);
void Motor_Set_0(uint8_t Address);
/*速度控制*/
void Motor_Speed_Control(uint8_t Address, int16_t speed, uint8_t aacelerated);
/*相对位置控制*/
void Motor_Relative_Position_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float angle, uint16_t Subdivision_Step, float Reduction_Ratio);
/*绝对位置控制*/
void Motor_Absolute_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float angle, bool Angle_Check_Sign, uint16_t Subdivision_Step, float Reduction_Ratio);
/*上电归零*/
void Motor_Absolute_Angle_To_0(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float angle_0, uint16_t Subdivision_Stepfloat, float Reduction_Ratio);
/*位置更新完成检测*/
bool Motor_Position_Done(uint8_t length, uint8_t data[]);
/*最终位置检测*/
static void Motor_Position_Done_Ckeck(uint8_t Address);
/*位置更新超时处理*/
static void Motor_Position_TIM_Start(uint8_t Address);
static void Motor_Position_TIM_Stop(uint8_t Address);
void Motor_Position_TIM_IT(TIM_HandleTypeDef *htim);
/*角度查询*/
void Motro_Position_Check(uint8_t Address);
static float Motro_Position_Calculate(uint8_t Address, uint8_t data[]);
/*编码器查询*/
void Motro_Encoder_Check(uint8_t Address);
static float Motro_Encoder_Calculate(uint8_t Address, uint8_t data[]);
/*电机通讯初始化*/
void Motro_Control_Init();

extern Motro_Control_ Motro_Control;
extern struct Queue Motro_Msg_Queue;

#endif
