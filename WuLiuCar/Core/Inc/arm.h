#ifndef ARM_H
#define ARM_H
#include <main.h>
#include "Motro_Control.h"

#define PI 3.1415926

/*电机参数*/
#define Arm_Subdivision_Step 256
#define Arm_Velocity 10
#define Arm_Accelerate 255

#define Arm_Rear_Address 0x05
#define Rear_Encoder_Angle_Correction 160
#define Rear_Angle_To_0_Set 24.5
#define Rear_Angle_To_0_Angle -13.4
#define Arm_Angle_a_Min -13.4
#define Arm_Angle_a_Max 69.1
#define Arm_Rear_Reduction_Ratio (float)827/225

#define Arm_Front_Address 0x06
#define Front_Encoder_Angle_Correction 80
#define Front_Angle_To_0_Set 52.6
#define Front_Angle_To_0_Angle 37.2
#define Arm_Angle_b_Min 23.6
#define Arm_Angle_b_Max 109.5
#define Arm_Front_Reduction_Ratio (float)1091/300

#define Arm_Pedestal_Address 0x07
#define Pedestal_Encoder_Angle_Correction 0
#define Pedestal_Angle_To_0_Set 175.87
#define Pedestal_Angle_To_0_Angle 0
#define Arm_Angle_c_Min -44
#define Arm_Angle_c_Max 44
#define Arm_Pedestal_Reduction_Ratio (float)3.67

/*手爪*/
#define Arm_Base_H 51.0
#define Arm_L 190.0
#define Arm_X_Diff 0
#define Arm_Y_Diff 100
#define Arm_Z_Diff -62

/*主控板位置*/
#define Controller_X_Min -140
#define Controller_X_Max -50
#define Controller_Y_Min 79
#define Controller_Y_Max 214
#define Controller_Z_Lim 30
/*上顶板位置*/
#define Top_Plate_X_Min -140
#define Top_Plate_X_Max 110
#define Top_Plate_Y_Min 0
#define Top_Plate_Y_Max 214
#define Top_Plate_Z_Lim 0
/*地面位置*/
#define Ground_Z_Lim -125

/*坐标系回0*/
#define Arm_Coord_X_0 0
#define Arm_Coord_Y_0 150
#define Arm_Coord_Z_0 5
typedef struct{
   double y;
   double z;
}Point2f_;

typedef struct{
   double x;
   double y;
   double z;
}Point3f_;

typedef struct{
   double a;
   double b;
   double c;
}Angle3f_;

typedef struct{
   Point3f_ O1;
   Point3f_ O2;
   Point3f_ O3;
   Point3f_ O4;
   Point3f_ PA;
   Point3f_ PB;
   Point3f_ PC;
   Point3f_ PD;
   double k;
   double b;
}Arm_Calculate_;

typedef struct{
   float X_Min;
   float X_Max;
   float Y_Min;
   float Y_Max;
   float Z_Min;
   float Z_Max;
}Range_;

typedef struct{
   bool Close_sign;
   bool debug_sign;
   float Limit_Angle_To_0[3];
   Arm_Calculate_ Calculate;
   Range_ Range;
}Arm_;

void Claw_Open();
void Claw_Close();
void Arm_Open();
void Arm_Close();
void Arm_MStep_Set(uint16_t num);
void Arm_Angle_Check();
void Arm_Encoder_Check();
void Arm_Position_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float angle, bool Angle_Check_Sign);

bool Arm_Angle_Calculate(Point3f_ *Point, Angle3f_ *Angle);
static bool Arm_Angle_Calculate_Range_Chack(Point3f_ *Point);
void Arm_Angle_Calculate_Init();
bool Arm_Coord_Control(Point3f_ *Point, Angle3f_ *Angle);
void Arm_Init();

extern Arm_ Arm;

#endif