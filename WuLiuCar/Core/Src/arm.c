#include "Arm.h"
#include "math.h" 
Arm_ Arm;

void Claw_Open()
{
    TIM10->CCR1 = 160;
}

void Claw_Close()
{
    TIM10->CCR1 = 80;
}

void Arm_Open()
{
    Motor_Open(Arm_Rear_Address);
    Motor_Open(Arm_Front_Address);
    Motor_Open(Arm_Pedestal_Address);
	Arm.Close_sign=false;
}

void Arm_Close()
{
    Motor_Close(Arm_Rear_Address);
    Motor_Close(Arm_Front_Address);
    Motor_Close(Arm_Pedestal_Address);
	Arm.Close_sign=true;
}

void Arm_MStep_Set(uint16_t num)  //设置细分步数 0：256细分
{	
    if(num >= 256)
        num = 0;

   Motor_MStep_Set(Arm_Rear_Address, num);
   Motor_MStep_Set(Arm_Front_Address, num);
    Motor_MStep_Set(Arm_Pedestal_Address, num);
}

void Arm_Set_0()  //设置零点
{	
    Motor_Set_0(Arm_Rear_Address);
    Motor_Set_0(Arm_Front_Address);
    Motor_Set_0(Arm_Pedestal_Address);
}

void Arm_Angle_Check()
{
    Motro_Position_Check(Arm_Rear_Address);
    Motro_Position_Check(Arm_Front_Address);
    Motro_Position_Check(Arm_Pedestal_Address);
}

void Arm_Encoder_Check()
{
    Motro_Encoder_Check(Arm_Rear_Address);
    Motro_Encoder_Check(Arm_Front_Address);
    Motro_Encoder_Check(Arm_Pedestal_Address);
}

void Arm_Position_Control(uint8_t Address, int8_t direction, uint16_t speed, uint8_t aacelerated, float angle, bool Angle_Check_Sign)
{
    Motor_Absolute_Control(Address, direction, speed, aacelerated, angle, Angle_Check_Sign, Arm_Subdivision_Step, Motro_Control.Reduction_Ratio[Address-1]); 
}

void Arm_Absolute_Angle_To_0(uint8_t Address)
{
    uint8_t ID = Address - 5;
    Motor_Absolute_Angle_To_0(Address, Clockwise, Arm_Velocity, Arm_Accelerate, Arm.Limit_Angle_To_0[ID], Arm_Subdivision_Step, Motro_Control.Reduction_Ratio[Address-1]);
}

void Arm_Absolute_Angle_To_0_All()
{
    uint8_t num = 0;
    uint16_t time_ms = 0;
    do{
        Arm_Absolute_Angle_To_0(Arm_Rear_Address);
				HAL_Delay(10);
        Arm_Absolute_Angle_To_0(Arm_Front_Address);
				HAL_Delay(10);
        Arm_Absolute_Angle_To_0(Arm_Pedestal_Address);
        do{
            HAL_Delay(1);
            time_ms++;
        }
        while(time_ms < 2000 && (Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1] != true || Motro_Control.Angle_Control.Done_sign[Arm_Front_Address-1] != true || Motro_Control.Angle_Control.Done_sign[Arm_Pedestal_Address-1] != true));
        if(Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1] && Motro_Control.Angle_Control.Done_sign[Arm_Front_Address-1] && Motro_Control.Angle_Control.Done_sign[Arm_Pedestal_Address-1])
            break;
        num++;
    }while(num < 5);
    Arm_Set_0();
    Arm_Angle_Check();
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

    Arm.Calculate.O1.x += Arm_X_Diff;
    Arm.Calculate.O1.y += Arm_Y_Diff;
    Arm.Calculate.O1.z += Arm_Z_Diff;
    Arm.Calculate.O2.x += Arm_X_Diff;
    Arm.Calculate.O2.y += Arm_Y_Diff;
    Arm.Calculate.O2.z += Arm_Z_Diff;
    Arm.Calculate.O3.x += Arm_X_Diff;
    Arm.Calculate.O3.y += Arm_Y_Diff;
    Arm.Calculate.O3.z += Arm_Z_Diff;
    Arm.Calculate.O4.x += Arm_X_Diff;
    Arm.Calculate.O4.y += Arm_Y_Diff;
    Arm.Calculate.O4.z += Arm_Z_Diff;

    Arm.Range.X_Min += Arm_X_Diff;
    Arm.Range.X_Max += Arm_X_Diff;
    Arm.Range.Y_Min += Arm_Y_Diff;
    Arm.Range.Y_Max += Arm_Y_Diff;
    Arm.Range.Z_Min += Arm_Z_Diff;
    Arm.Range.Z_Max += Arm_Z_Diff;
    if(Arm.Range.Z_Min < Ground_Z_Lim)  Arm.Range.Z_Min = Ground_Z_Lim;

    Arm.Calculate.k = (Arm.Calculate.PB.z - Arm.Calculate.PA.z) / (Arm.Calculate.PB.y - Arm.Calculate.PA.y);
    Arm.Calculate.b = Arm.Calculate.PA.z - Arm.Calculate.k * Arm.Calculate.PA.y;
}

static bool Arm_Angle_Calculate_Range_Chack(Point3f_ *Point)
{
    bool sign = false;

    double x = Point->x;
    double y = Point->y;
    double z = Point->z;
    
    /*是否在底座转角范围内*/
    double c = atan((x - Arm_X_Diff) / (y - Arm_Y_Diff)) * 180 / PI;
    if(c < Arm_Angle_c_Min || c > Arm_Angle_c_Max)
    {
        goto OutRange;
    }
    /*如果在主控板区域*/
    if(x >= Controller_X_Min && x <= Controller_X_Max && y >= Controller_Y_Min && y <= Controller_Y_Max)
    {
        if(z < Controller_Z_Lim)   goto OutRange;
    }
    /*如果在上顶板区域*/
    else if(x >= Top_Plate_X_Min && x <= Top_Plate_X_Max && y >= Top_Plate_Y_Min && y <= Top_Plate_Y_Max)
    {
        if(z < Top_Plate_Z_Lim)   goto OutRange;
    }
    else
    {
        if(z < Ground_Z_Lim)    goto OutRange;
    }
    /*是否在范围上半部分*/
    /*圆锥面*/
    double z_cone = Arm.Calculate.k * pow(pow(x - Arm_X_Diff, 2) + pow(y - Arm_Y_Diff, 2), 0.5) + Arm.Calculate.b + Arm_Z_Diff;
    if(z >= z_cone)
    {
        /*点到O1球心的距离*/
        double R1 = pow(pow(x - Arm.Calculate.O1.x, 2) + pow(y - Arm.Calculate.O1.y, 2) + pow(z - Arm.Calculate.O1.z, 2), 0.5);
        /*点到O2球心的距离*/
        double R2 = pow(pow(x - Arm.Calculate.O2.x, 2) + pow(y - Arm.Calculate.O2.y, 2) + pow(z - Arm.Calculate.O2.z, 2), 0.5);
        if(R1 < Arm_L || R2 > Arm_L)  goto OutRange;
    }
    else
    {
        /*点到O3球心的距离*/
        double R3 = pow(pow(x - Arm.Calculate.O3.x, 2) + pow(y - Arm.Calculate.O3.y, 2) + pow(z - Arm.Calculate.O3.z, 2), 0.5);
        /*点到O4球心的距离*/
        double R4 = pow(pow(x - Arm.Calculate.O4.x, 2) + pow(y - Arm.Calculate.O4.y, 2) + pow(z - Arm.Calculate.O4.z, 2), 0.5);
        if(R3 > Arm_L || R4 < Arm_L)  goto OutRange;
    }

    sign = true;

    OutRange:
    return sign;
}

bool Arm_Angle_Calculate(Point3f_ *Point, Angle3f_ *Angle)
{
    bool sign = false;
	double x = Point->x - Arm_X_Diff;
	double y = Point->y - Arm_Y_Diff;
	double z = Point->z - Arm_Z_Diff;
	
	double L = pow(pow(x, 2) + pow(y, 2), 0.5);
    if(Arm_Angle_Calculate_Range_Chack(Point) == true)
    {
        double L_ = pow(pow(L, 2) + pow(Arm_Base_H - z, 2), 0.5);
        
        double A = atan((Arm_Base_H - z) / L);
        double B = acos(L_ / (2*Arm_L));
        
        Angle->a = (A - B + PI/2) * 180 / PI;
        Angle->b = (-A - B + PI/2) * 180 / PI;
        Angle->c = atan(x / y) * 180 / PI;
        sign = true;
    }

    return sign;
}

bool Arm_Coord_Control(Point3f_ *Point, Angle3f_ *Angle)
{
	bool sign = false;
    if(Arm_Angle_Calculate(Point, Angle) == true)
    {
        double Angle_a = Motro_Control.Angle.Value[Arm_Rear_Address - 1];
        double Angle_b = Motro_Control.Angle.Value[Arm_Front_Address - 1];
        double Angle_c = Motro_Control.Angle.Value[Arm_Pedestal_Address - 1];
        double Angle_a_diff = fabs(Angle_a - Angle->a);
        double Angle_b_diff = fabs(Angle_b - Angle->b);
        double Angle_c_diff = fabs(Angle_c - Angle->c);
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
        if( Angle->a >= Arm_Angle_a_Min && Angle->a <= Arm_Angle_a_Max && 
            Angle->b >= Arm_Angle_b_Min && Angle->b <= Arm_Angle_b_Max && 
            Angle->c >= Arm_Angle_c_Min && Angle->c <= Arm_Angle_c_Max)
        {
            Arm_Position_Control(Arm_Rear_Address, Clockwise, Arm_Velocity * 1, Arm_Accelerate, Angle->a, true);
            Arm_Position_Control(Arm_Front_Address, Clockwise, Arm_Velocity * 1, Arm_Accelerate, Angle->b, true);
            Arm_Position_Control(Arm_Pedestal_Address, Clockwise, Arm_Velocity * 1, Arm_Accelerate, -Angle->c, true);
            sign = true;
        }
    }
    return sign;
}

void Arm_Init()
{
    Arm.Limit_Angle_To_0[0] = Rear_Angle_To_0_Set;
    Arm.Limit_Angle_To_0[1] = Front_Angle_To_0_Set;
    Arm.Limit_Angle_To_0[2] = Pedestal_Angle_To_0_Set;

    Arm_MStep_Set(Arm_Subdivision_Step);
    Arm_Absolute_Angle_To_0_All();
}



