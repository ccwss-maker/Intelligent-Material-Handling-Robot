#ifndef ARM_H
#define ARM_H
#include <main.h>


#define PI 3.1415926

/*电机参数*/
#define Arm_Subdivision_Step 256
#define Arm_Velocity_Default 10
#define Arm_Accelerate 255

#define Arm_Address_All 0x00

#define Arm_Rear_Address 0x01
#define Rear_Angle_Correction -282.8
#define Rear_Encoder 360 - Sensor_Msg.Encoder_Data[0].angle
#define Rear_Angle Angle_Calculate(Rear_Encoder, Rear_Angle_Correction, 260)
#define Rear_Angle_To_0_Set -33
#define Arm_Angle_a_Min -42.3
#define Arm_Angle_a_Max 90
#define Arm_Rear_Reduction_Ratio 3.69239905

#define Arm_Front_Address 0x02
#define Front_Angle_Correction -234.8
#define Front_Encoder 360 - Sensor_Msg.Encoder_Data[1].angle
#define Front_Angle Angle_Calculate(Front_Encoder, Front_Angle_Correction, 360)
#define Front_Angle_To_0_Set 61
#define Arm_Angle_b_Min 8
#define Arm_Angle_b_Max 118
#define Arm_Front_Reduction_Ratio 3.713764813

#define Arm_Pedestal_Address 0x03
#define Pedestal_Angle_Correction -63
#define Pedestal_Encoder Sensor_Msg.Encoder_Data[2].angle
#define Pedestal_Angle  Angle_Calculate(Pedestal_Encoder, Pedestal_Angle_Correction, 270)
#define Pedestal_Angle_To_0_Set 0
#define Arm_Angle_c_Min -90
#define Arm_Angle_c_Max 90
#define Arm_Pedestal_Reduction_Ratio 3.711524164

/*手爪*/
#define Arm_Base_H 59.0
#define Arm_L 190.0
#define Arm_X_Diff 0
#define Arm_Y_Diff 97
#define Arm_Z_Diff -26.1

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
#define Arm_Coord_Y_0 160
#define Arm_Coord_Z_0 100
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
   float Angle[3];
   Arm_Calculate_ Calculate;
   Range_ Range;
}Arm_;

float Angle_Calculate(float Encoder, float Correction, float max);
void Claw_Open();
void Claw_Close();
void Arm_Open();
void Arm_Close();
void Arm_MStep_Set(uint16_t num);

Point3f_ Arm_Real_Position_Calculate(Angle3f_ Angle);
void Arm_Absolute_Angle_To_0_All();
bool Arm_Angle_Calculate(Point3f_ *Point, Angle3f_ *Angle);
void Arm_Angle_Calculate_Init();
bool Arm_Coord_Control(Point3f_ *Point, Angle3f_ *Angle, uint8_t Velocity);
void Arm_Coord_Control_f(float x, float y, float z, uint8_t Velocity, void (*While)(void));
void Arm_Init();

extern Arm_ Arm;

#endif