#include "Arm.h"
#include "tim.h"
#include "ble.h"
#include "math.h" 
#include "Master_Slave_Communication.h"
#include "Motro_Control_Basic.h"
#include "Motro_Control_Arm.h"
Arm_ Arm;

float Angle_Calculate(float Encoder, float Correction, float max)
{
    float result;
    result = Encoder + Correction;
    if(result < 0)          result += 360;
    else if(result > 360)   result -= 360;
    if(result > max) result -= 360;
    return result;
}
void Claw_Open()
{
    TIM10->CCR1 = 250;
}

void Claw_Close()
{
    TIM10->CCR1 = 186;
}

void Arm_Open()
{
    Motor_Open(UART_Arm, Arm_Address_All);
    Arm.Close_sign=false;
}

void Arm_Close()
{
   Motor_Close(UART_Arm, Arm_Address_All);
	Arm.Close_sign=true;
}

void Arm_MStep_Set(uint16_t num)  //设置细分步数 0：256细分
{	
    if(num >= 256)
        num = 0;
    Motor_MStep_Set(UART_Arm, Arm_Address_All, num);
}

void Arm_Absolute_Angle_To_0(uint8_t Address)
{
    float Angle_0;
    switch(Address)
    {
        case Arm_Rear_Address:
            Angle_0 = Rear_Angle_To_0_Set;
            break;
        case Arm_Front_Address:
            Angle_0 = Front_Angle_To_0_Set;
            break;
        case Arm_Pedestal_Address:
            Angle_0 = Pedestal_Angle_To_0_Set;
            break;
    }
    Motor_Absolute_Control(Address, Arm_Velocity_Default, Arm_Accelerate, Angle_0);
}

void Arm_Absolute_Angle_To_0_All()
{
    while(Sensor_Msg.Encoder_Data[0].angle==0 && Sensor_Msg.Encoder_Data[1].angle==0 && Sensor_Msg.Encoder_Data[2].angle==0);
    Point3f_ Point;
    Angle3f_ Angle;
    Point.x = Arm_Coord_X_0;
    Point.y = Arm_Coord_Y_0;
    Point.z = Arm_Coord_Z_0;
    Arm_Coord_Control(&Point, &Angle, Arm_Velocity_Default);

    // Arm_Absolute_Angle_To_0(Arm_Rear_Address);
    // Arm_Absolute_Angle_To_0(Arm_Front_Address);
    // Arm_Absolute_Angle_To_0(Arm_Pedestal_Address);
}

void Arm_Angle_Calculate_Init()
{
    Arm.Calculate.O1.x = 0;
    Arm.Calculate.O1.y = Arm_L * sin(Arm_Angle_a_Min * PI / 180.0);
    Arm.Calculate.O1.z = Arm_Base_H + Arm_L * cos(Arm_Angle_a_Min * PI / 180.0);
    Arm.Calculate.O2.x = 0;
    Arm.Calculate.O2.y = Arm_L * sin(Arm_Angle_b_Max * PI / 180.0);
    Arm.Calculate.O2.z = Arm_Base_H - Arm_L * cos(Arm_Angle_b_Max * PI / 180.0);
    Arm.Calculate.O3.x = 0;
    Arm.Calculate.O3.y = Arm_L * sin(Arm_Angle_a_Max * PI / 180.0);
    Arm.Calculate.O3.z = Arm_Base_H + Arm_L * cos(Arm_Angle_a_Max * PI / 180.0);
    Arm.Calculate.O4.x = 0;
    Arm.Calculate.O4.y = Arm_L * sin(Arm_Angle_b_Min * PI / 180.0);
    Arm.Calculate.O4.z = Arm_Base_H - Arm_L * cos(Arm_Angle_b_Min * PI / 180.0);
   
    Arm.Calculate.PA.x = 0;
    Arm.Calculate.PA.y = Arm_L * (sin(Arm_Angle_a_Min * PI / 180.0) + sin(Arm_Angle_b_Min * PI / 180.0));
    Arm.Calculate.PA.z = Arm_Base_H + Arm_L * (cos(Arm_Angle_a_Min * PI / 180.0) - cos(Arm_Angle_b_Min * PI / 180.0));
    Arm.Calculate.PB.x = 0;
    Arm.Calculate.PB.y = Arm_L * (sin(Arm_Angle_a_Max * PI / 180.0) + sin(Arm_Angle_b_Max * PI / 180.0));
    Arm.Calculate.PB.z = Arm_Base_H + Arm_L * (cos(Arm_Angle_a_Max * PI / 180.0) - cos(Arm_Angle_b_Max * PI / 180.0));
    Arm.Calculate.PC.x = 0;
    Arm.Calculate.PC.y = Arm_L * (sin(Arm_Angle_a_Max * PI / 180.0) + sin(Arm_Angle_b_Min * PI / 180.0));
    Arm.Calculate.PC.z = Arm_Base_H + Arm_L * (cos(Arm_Angle_a_Max * PI / 180.0) - cos(Arm_Angle_b_Min * PI / 180.0));
    Arm.Calculate.PD.x = 0;
    Arm.Calculate.PD.y = Arm_L * (sin(Arm_Angle_a_Min * PI / 180.0) + sin(Arm_Angle_b_Max * PI / 180.0));
    Arm.Calculate.PD.z = Arm_Base_H + Arm_L * (cos(Arm_Angle_a_Min * PI / 180.0) - cos(Arm_Angle_b_Max * PI / 180.0));

    /*计算机械臂边界范围*/
    Arm.Range.Y_Min = Arm.Calculate.PA.y;
    /*判断2号圆和3号圆，哪个圆心y大*/
    double y_max = Arm.Calculate.O2.y;
    if(Arm.Calculate.O3.y > y_max)  y_max = Arm.Calculate.O3.y;
    Arm.Range.Y_Max = y_max + Arm_L;
    Arm.Range.X_Min = Arm.Range.Y_Max * sin(Arm_Angle_c_Min * PI / 180.0);
    Arm.Range.X_Max = Arm.Range.Y_Max * sin(Arm_Angle_c_Max * PI / 180.0);
    Arm.Range.Z_Min = Arm.Calculate.PC.z;
    Arm.Range.Z_Max = Arm.Calculate.PD.z;

    Arm.Range.X_Min += Arm_X_Diff;
    Arm.Range.X_Max += Arm_X_Diff;
    Arm.Range.Y_Min += Arm_Y_Diff;
    Arm.Range.Y_Max += Arm_Y_Diff;
    Arm.Range.Z_Min += Arm_Z_Diff;
    Arm.Range.Z_Max += Arm_Z_Diff;
    if(Arm.Range.Z_Min < Ground_Z_Lim)  Arm.Range.Z_Min = Ground_Z_Lim;
}

static bool Arm_Angle_Calculate_Range_Chack_1(Point3f_ *Point)
{
    bool sign = false;

    double x = Point->x;
    double y = Point->y;
    double z = Point->z;

    /*如果在主控板区域*/
    if(x >= Controller_X_Min && x <= Controller_X_Max && y >= Controller_Y_Min && y <= Controller_Y_Max)
    {
        if(z > Controller_Z_Lim)   sign = true;
    }
    /*如果在上顶板区域*/
    else if(x >= Top_Plate_X_Min && x <= Top_Plate_X_Max && y >= Top_Plate_Y_Min && y <= Top_Plate_Y_Max)
    {
        if(z > Top_Plate_Z_Lim)   sign = true;
    }
    else
    {
        if(z > Ground_Z_Lim)    sign = true;
    }
    
    return sign;
}

static bool Arm_Angle_Calculate_Range_Chack_2(Angle3f_ *Angle)
{
    bool sign = false;
    /* 判断是否在范围内 */
    if( Angle->a >= Arm_Angle_a_Min && Angle->a <= Arm_Angle_a_Max && 
        Angle->b >= Arm_Angle_b_Min && Angle->b <= Arm_Angle_b_Max && 
        Angle->c >= Arm_Angle_c_Min && Angle->c <= Arm_Angle_c_Max)
        sign = true;

    return sign;
}

bool Arm_Angle_Calculate(Point3f_ *Point, Angle3f_ *Angle)
{
    bool sign = false;
	double x = Point->x - Arm_X_Diff;
	double y = Point->y - Arm_Y_Diff;
	double z = Point->z - Arm_Z_Diff;
	double l = pow(pow(x, 2) + pow(y, 2), 0.5);
    if(Arm_Angle_Calculate_Range_Chack_1(Point) == true)
    {
        double l_ = pow(pow(l, 2) + pow(Arm_Base_H - z, 2), 0.5);
        
        double A = atan((Arm_Base_H - z) / l);
        double B = acos(l_ / (2*Arm_L));
        
        Angle->a = (A - B + PI/2) * 180 / PI;
        Angle->b = (-A - B + PI/2) * 180 / PI;
        Angle->c = atan(x / y) * 180 / PI;
        if(Arm_Angle_Calculate_Range_Chack_2(Angle) == true)
            sign = true;
    }
    return sign;
}

Point3f_ Arm_Real_Position_Calculate(Angle3f_ Angle)
{
    Point3f_ Point;
    float A = ((Angle.a - Angle.b) * PI / 180.0)/2;
    float B = ((Angle.a + Angle.b) * PI / 180.0 + PI)/2;
    float l_ = fabs(2 * Arm_L * cos(B));
    float l = fabs(l_ * cos(A));
    float a2 = acos(l_/(2*Arm_L))*180/PI;
    Point.x = l * sin(Angle.c * PI / 180.0)+Arm_X_Diff;
    Point.y = l * cos(Angle.c * PI / 180.0)+Arm_Y_Diff;
    if(Angle.a+a2 > 90) Point.z = Arm_Base_H - pow(pow(l_, 2) - pow(l, 2), 0.5) + Arm_Z_Diff;
    else                Point.z = Arm_Base_H + pow(pow(l_, 2) - pow(l, 2), 0.5) + Arm_Z_Diff;
    return Point;
}

bool Arm_Coord_Control(Point3f_ *Point, Angle3f_ *Angle, uint8_t Velocity)
{
	bool sign = false;
    if(Arm_Angle_Calculate(Point, Angle) == true)
    {
        double Angle_a_diff = fabs(Rear_Angle - Angle->a);
        double Angle_b_diff = fabs(Front_Angle - Angle->b);
        double Angle_c_diff = fabs(Pedestal_Angle - Angle->c);
        double k_a, k_b, k_c;
        if(Angle_a_diff >= Angle_b_diff && Angle_a_diff >= Angle_c_diff)
        {
            k_a = 1.0;
            k_b = Angle_b_diff / Angle_a_diff;
            k_c = Angle_c_diff / Angle_a_diff;
        }
        else if(Angle_b_diff >= Angle_a_diff && Angle_b_diff >= Angle_c_diff)
        {
            k_a = Angle_a_diff / Angle_b_diff;
            k_b = 1.0;
            k_c = Angle_c_diff / Angle_b_diff;
        }
        else if(Angle_c_diff >= Angle_a_diff && Angle_c_diff >= Angle_b_diff)
        {
            k_a = Angle_a_diff / Angle_c_diff;
            k_b = Angle_b_diff / Angle_c_diff;
            k_c = 1.0;
        }
        Motor_Absolute_Control(Arm_Rear_Address, Velocity * k_a, Arm_Accelerate, Angle->a);
        Motor_Absolute_Control(Arm_Front_Address, Velocity * k_b, Arm_Accelerate, Angle->b);
        Motor_Absolute_Control(Arm_Pedestal_Address, Velocity * k_c, Arm_Accelerate, Angle->c);
        sign = true;
    }

    return sign;
}

void Arm_Coord_Control_f(float x, float y, float z, uint8_t Velocity, void (*While)(void))
{
    Point3f_ Point={x, y, z};
    Angle3f_ Angle;
    if(Arm_Coord_Control(&Point, &Angle, Velocity))
    {
        TIM4->CNT = 0;
        while(fabs(Angle.a - Rear_Angle)>1 || fabs(Angle.b - Front_Angle)>1 || fabs(Angle.c - Pedestal_Angle)>1)
        {
            if(TIM4->CNT > 10000)   Arm_Coord_Control(&Point, &Angle, Velocity);
            While();
        }
    }
}

void Arm_Init()
{
    Arm_Open();
    Arm_MStep_Set(Arm_Subdivision_Step);
    Arm_Absolute_Angle_To_0_All();
    Claw_Open();
}



