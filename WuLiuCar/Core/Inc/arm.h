#ifndef ARM_H
#define ARM_H
#include <main.h>
#include "Motro_Control.h"
#define Arm_Subdivision_Step 256
#define Arm_Reduction_Ratio 3.7
#define Arm_Velocity 20
#define Arm_Accelerate 150

#define Arm_Rear_Address 0x05
#define Arm_Front_Address 0x06
#define Arm_Pedestal_Address 0x07

/*18 ~ 310*/
#define Rear_Encoder_Angle_Correction 200
#define Rear_Angle_To_0_Set 20

/*3 ~ 340*/
#define Front_Encoder_Angle_Correction 220
#define Front_Angle_To_0_Set 40

/*0 ~ 360*/
#define Pedestal_Encoder_Angle_Correction 0
#define Pedestal_Angle_To_0_Set 180

typedef struct{
   bool Close_sign;
   bool debug_sign;
   float Limit_Angle_To_0[2];
}Arm_;

void Arm_Open();
void Arm_Close();
void Arm_MStep_Set(uint16_t num);
void Arm_Angle_Check();
void Arm_Encoder_Check();
void Arm_Position_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float angle, bool Angle_Check_Sign);

void Arm_Init();

extern Arm_ Arm;

#endif
