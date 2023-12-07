#include "Arm.h"

Arm_ Arm;

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
    if(Address == Arm_Rear_Address || Address == Arm_Front_Address )
        Motor_Absolute_Control(Address, direction, speed, aacelerated, angle, Angle_Check_Sign, Arm_Subdivision_Step, Arm_Reduction_Ratio);
    else if(Address == Arm_Pedestal_Address)
        Motor_Absolute_Control(Address, direction, speed, aacelerated, angle, Angle_Check_Sign, Arm_Subdivision_Step, 1);
}

void Arm_Absolute_Angle_To_0(uint8_t Address)
{
    uint8_t ID = Address - 5;
    if(Address == Arm_Rear_Address || Address == Arm_Front_Address )
        Motor_Absolute_Angle_To_0(Address, Clockwise, Arm_Velocity, Arm_Accelerate, Arm.Limit_Angle_To_0[ID], Arm_Subdivision_Step, Arm_Reduction_Ratio);
    else if(Address == Arm_Pedestal_Address)
        Motor_Absolute_Angle_To_0(Address, Clockwise, Arm_Velocity, Arm_Accelerate, Arm.Limit_Angle_To_0[ID], Arm_Subdivision_Step, 1);
}

void Arm_Absolute_Angle_To_0_All()
{
    uint8_t num = 0;
    uint16_t time_ms = 0;
    do{
        Arm_Absolute_Angle_To_0(Arm_Rear_Address);
        Arm_Absolute_Angle_To_0(Arm_Front_Address);
        Arm_Absolute_Angle_To_0(Arm_Pedestal_Address);
        do{
            HAL_Delay(1);
            time_ms++;
        }
        while(time_ms < 2000 && (Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1] != true || Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1] != true || Motro_Control.Angle_Control.Done_sign[Arm_Pedestal_Address-1] != true));
        if(Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1] && Motro_Control.Angle_Control.Done_sign[Arm_Rear_Address-1] && Motro_Control.Angle_Control.Done_sign[Arm_Pedestal_Address-1])
            break;
        num++;
    }while(num < 5);
    Arm_Set_0();
    Arm_Angle_Check();
}

void Arm_Init()
{
    Arm.Limit_Angle_To_0[0] = Rear_Angle_To_0_Set;
    Arm.Limit_Angle_To_0[1] = Front_Angle_To_0_Set;

    Arm_MStep_Set(Arm_Subdivision_Step);
//    Arm_Absolute_Angle_To_0_All();
}



