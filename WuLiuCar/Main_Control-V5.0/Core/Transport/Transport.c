#include "Transport.h"
#include "lv_app_transport.h"
#include "lv_app.h"
#include "wheel.h"
#include "Arm.h"
#include "Master_Slave_Communication.h"
#include "stdlib.h"
#include "Camera.h"
Transport_ Transport;

void Transport_Stop()
{
    Wheel_Speed_To_0(255);
    PID_Stop();
    Wheel_Close();
    Arm_Coord_Control_f(Arm_Coord_X_0, Arm_Coord_Y_0, Arm_Coord_Z_0, 15, &While);/*机械臂开始归位*/
    Transport.sign_Running = false;
    lv_obj_clear_state(lv_obj_Transport.btn_start, LV_STATE_CHECKED);
    lv_obj_t * label = lv_obj_get_child(lv_obj_Transport.btn_start, 0);
    lv_label_set_text(label, "启动");
}

/*竖直中部对齐*/
void Align_Mid_vertical(uint8_t num, uint8_t direction)
{
    if(Transport.sign_Running)
    {
        float speed;
        uint8_t data = 1 << (num-1);
        switch(direction)
        {
            case Front:
                speed = 0.5;
                break;
            case Back:
                speed = -0.5;
                break;
        }

        Wheel_Closed_Loop_Spd_Set(0, speed, 0, 200);
        while((Sensor_Msg.Tracking_Data[Tracking_Left] & data) == 0)
        {
            While();
            if(!Transport.sign_Running) break;
        }
        Wheel_Speed_To_0(250);
    }
}

/*水平中部对齐*/
void Align_Mid_horizontal(uint8_t num, uint8_t direction)
{
    if(Transport.sign_Running)
    {
        float speed;
        uint8_t data = 1 << (num-1);
        switch(direction)
        {
            case Left:
                speed = -0.5;
                break;
            case Right:
                speed = 0.5;
                break;
        }

        Wheel_Closed_Loop_Spd_Set(speed, 0, 0, 200);
        while((Sensor_Msg.Tracking_Data[Tracking_Front] & data) == 0)
        {
            While();
            if(!Transport.sign_Running) break;
        }
        Wheel_Speed_To_0(250);
    }
}

/*竖直顶部对齐*/
void Align_vertical(uint8_t num)
{
    if(Transport.sign_Running)
    {
        float speed;
        int8_t Tracking_Data;
        switch(num)
        {
            case Front:
                speed = 0.5;
                Tracking_Data = Tracking_Front;
                break;
            case Back:
                speed = -0.5;
                Tracking_Data = Tracking_Back;
                break;
        }
        Wheel_Closed_Loop_Spd_Set(0, speed, 0, 200);
        while(Sensor_Msg.Tracking_Data[Tracking_Data] != 0xff)
        {
            While();
            if(!Transport.sign_Running) break;
        }
        Wheel_Speed_To_0(250);
    }
}
/*水平顶部对齐*/
void Align_horizontal(uint8_t num)
{
    if(Transport.sign_Running)
    {
        float speed;
        int8_t Tracking_Data;
        switch(num)
        {
            case Left:
                speed = -0.5;
                Tracking_Data = Tracking_Left;
                break;
            case Right:
                speed = 0.5;
                Tracking_Data = Tracking_Right;
                break;
        }
        Wheel_Closed_Loop_Spd_Set(speed, 0, 0, 200);
        while(Sensor_Msg.Tracking_Data[Tracking_Data] != 0xff)
        {
            While();
            if(!Transport.sign_Running) break;
        }
        Wheel_Speed_To_0(250);
    }
}

/*竖直移动*/
void Move_vertical(int8_t num)
{
    if(Transport.sign_Running)
    {
        float speed;
        int8_t Tracking_Data;
        if(num>0) 
        {
            speed = 1;
            Tracking_Data = Tracking_Back;
        }
        else      
        {
            speed = -1;
            Tracking_Data = Tracking_Front;
        }
        num = abs(num);
        Wheel_Closed_Loop_Spd_Set(0, speed, 0, 200);
        for(uint8_t i=0; i<num; i++)
        {
            while(Sensor_Msg.Tracking_Data[Tracking_Data] != 0xff)
            {
                While();
                if(!Transport.sign_Running) break;
            }
            while(Sensor_Msg.Tracking_Data[Tracking_Data] == 0xff)
            {
                While();
                if(!Transport.sign_Running) break;
            }
        }
        Wheel_Speed_To_0(240);
    }
}

/*水平移动*/
void Move_horizontal(int8_t num)
{
    if(Transport.sign_Running)
    {
        float speed;
        int8_t Tracking_Data;
        if(num>0) 
        {
            speed = 1;
            Tracking_Data = Tracking_Left;
        }
        else      
        {
            speed = -1;
            Tracking_Data = Tracking_Right;
        }
        num = abs(num);
        Wheel_Closed_Loop_Spd_Set(speed, 0, 0, 200);
        for(uint8_t i=0; i<num; i++)
        {
            while(Sensor_Msg.Tracking_Data[Tracking_Data] != 0xff)
            {
                While();
                if(!Transport.sign_Running) break;
            }
            while(Sensor_Msg.Tracking_Data[Tracking_Data] == 0xff)
            {
                While();
                if(!Transport.sign_Running) break;
            }
        }
        Wheel_Speed_To_0(240);
    }
}

void Move(int8_t hor, int8_t ver, uint8_t sign)
{
    if(Transport.sign_Running)
    {
        if(sign == Ver_First)
        {
            Move_vertical(ver);
            HAL_Delay(500);
            Move_horizontal(hor);
        }
        else if(sign == Hor_First)
        {
            Move_horizontal(hor);
            HAL_Delay(500);
            Move_vertical(ver);
        }
    }
}

void Transport_QR_Code_Detection()
{
    if(Transport.sign_Running)
    {
        Arm_Coord_Control_f(-40, 280, 180, 15, &While);         /*机械臂开始移动至二维码左上角*/
        do{
            Camera_QR_Code_Detection();                             /*发送识别二维码指令*/
            Arm_Coord_Control_f(40, 280, 180, 2, &While);           /*机械臂开始移动至二维码右上角*/
            if(Transport.Camera_Object_Order[0] != Object_None)  break;
            Camera_QR_Code_Detection();                             /*发送识别二维码指令*/
            Arm_Coord_Control_f(40, 280, 120, 2, &While);           /*机械臂开始移动至二维码右下角*/
            if(Transport.Camera_Object_Order[0] != Object_None)  break;
            Camera_QR_Code_Detection();                             /*发送识别二维码指令*/
            Arm_Coord_Control_f(-40, 280, 120, 2, &While);          /*机械臂开始移动至二维码左下角*/
            if(Transport.Camera_Object_Order[0] != Object_None)  break;
            Camera_QR_Code_Detection();                             /*发送识别二维码指令*/
            Arm_Coord_Control_f(-40, 280, 180, 2, &While);          /*机械臂开始移动至二维码左上角*/                                      /*显示识别结果*/
            if(!Transport.sign_Running) break;
        }while(Transport.Camera_Object_Order[0] == Object_None);
        Arm_Coord_Control_f(Arm_Coord_X_0, Arm_Coord_Y_0, Arm_Coord_Z_0, 15, &While);/*机械臂开始归位*/
    }
}

void Transport_Obiect_Detection(uint8_t* Camera_Object_Detect, int8_t x_min, int8_t x_max)
{
    if(Transport.sign_Running)
    {
        Arm_Coord_Control_f(x_min, 280, 250, Arm_Velocity_Default, &While);   /*机械臂开始抬起,准备检测右一物料颜色*/
        do{
            Camera_Object_Detection();                                      /*发送物体识别指令*/
            Arm_Coord_Control_f(x_max, 280, 250, 2, &While);                   /*机械臂开始移动至物体右侧*/
            if(*Camera_Object_Detect != Object_None)  break;
            Camera_QR_Code_Detection();                                     /*发送物体识别指令*/
            Arm_Coord_Control_f(x_min, 280, 250, 2, &While);                  /*机械臂开始移动至物体左侧*/
            if(!Transport.sign_Running) break;
        }while(*Camera_Object_Detect == Object_None);
    }
}

void Transport_Obiect_Detection_All()
{
    Transport.Camera_Object_Detecting_Num = Detecting_Left;
    Transport_Obiect_Detection(&Transport.Camera_Object_Detect[0], -80, -60); /*检测中间物料颜色*/
    Camera_Msg_Clear();
    Transport.Camera_Object_Detecting_Num = Detecting_Mid;
    Transport_Obiect_Detection(&Transport.Camera_Object_Detect[1], -20, 10); /*检测中间物料颜色*/
    Camera_Msg_Clear();
    Transport.Camera_Object_Detecting_Num = Detecting_Right;
    Transport_Obiect_Detection(&Transport.Camera_Object_Detect[2], 50, 70); /*检测中间物料颜色*/
    Arm_Coord_Control_f(Arm_Coord_X_0, Arm_Coord_Y_0, Arm_Coord_Z_0, 15, &While);/*机械臂开始归位*/
}

void Transport_Obiect_Catch(uint8_t num_color)
{
    Claw_Open();                                        /*打开爪子*/
    uint8_t Num_Pont_Car;
    Point3f_ Point_Object_Detect[2];
    if(Transport.Camera_Object_Order[num_color] == Transport.Camera_Object_Detect[0])/*判断取料位置*/
    {
        Point_Object_Detect[0].x = -94;
        Point_Object_Detect[0].y = 350;
        Point_Object_Detect[0].z = 95;
        Point_Object_Detect[1].x = -103;
        Point_Object_Detect[1].y = 384;
        Point_Object_Detect[1].z = 95;
    }
    else if(Transport.Camera_Object_Order[num_color] == Transport.Camera_Object_Detect[1])
    {
        Point_Object_Detect[0].x = 5;
        Point_Object_Detect[0].y = 324;
        Point_Object_Detect[0].z = 95;
        Point_Object_Detect[1].x = 5;
        Point_Object_Detect[1].y = 367;
        Point_Object_Detect[1].z = 95;
    }
    else if(Transport.Camera_Object_Order[num_color] == Transport.Camera_Object_Detect[2])
    {
        Point_Object_Detect[0].x = 100;
        Point_Object_Detect[0].y = 330;
        Point_Object_Detect[0].z = 95;
        Point_Object_Detect[1].x = 110;
        Point_Object_Detect[1].y = 367;
        Point_Object_Detect[1].z = 95;
    }

    switch(Transport.Camera_Object_Order[num_color])    /*判断放料位置*/
    {
        case Object_Red:
            Num_Pont_Car = 0;
            break;
        case Object_Green:
            Num_Pont_Car = 1;
            break;
        case Object_Blue:
            Num_Pont_Car = 2;
            break;
    }
    Arm_Coord_Control_f(Point_Object_Detect[0].x, Point_Object_Detect[0].y, Point_Object_Detect[0].z, 10, &While);           /*机械臂开始移动至物料前*/
    Arm_Coord_Control_f(Point_Object_Detect[1].x, Point_Object_Detect[1].y, Point_Object_Detect[1].z, 2, &While);         /*机械臂开始移动至物料中心*/
    Claw_Close();                                       /*关闭爪子*/
    HAL_Delay(500);
    Arm_Coord_Control_f(Point_Object_Detect[0].x, Point_Object_Detect[0].y-50, Point_Object_Detect[0].z, 10, &While);         /*机械臂退出原料区*/
    Arm_Coord_Control_f(Transport.Point_Car[Num_Pont_Car].x, Transport.Point_Car[Num_Pont_Car].y, Transport.Point_Car[Num_Pont_Car].z, 10, &While);           /*机械臂开始移动至放料位置*/
    Claw_Open();                                        /*打开爪子*/
    HAL_Delay(500);
    Arm_Coord_Control_f(Transport.Point_Car[Num_Pont_Car].x, Transport.Point_Car[Num_Pont_Car].y, Transport.Point_Car[Num_Pont_Car].z+20, 10, &While);           /*机械臂开始退出放料位置*/
    Arm_Coord_Control_f(Arm_Coord_X_0, Arm_Coord_Y_0, Arm_Coord_Z_0, 10, &While);/*机械臂开始归位*/
}

void Transport_main()
{
    while (Transport.sign_Running)
    {
        Wheel_Closed_Loop_Init();
        PID_Start();
        /*初始化*/
        char string[10];  
        /*起点 -> 二维码*/
        Move(-2,-1, Ver_First);                            /*移动：左2,后1*/
        Align_vertical(Front);                             /*对齐前边线*/   
        sprintf(string, "二维码");                          /*状态更新*/
        lv_label_set_text(lv_obj_Transport.State.label, string);
        /*二维码检测*/
        Transport_QR_Code_Detection();                      /*二维码检测*/
        /*二维码 -> 原料区*/
        Align_vertical(Back);                               /*对齐后边线*/
        Move_horizontal(-3);                                /*移动：左3*/
        Align_vertical(Front);                              /*对齐前边线*/
        Align_Mid_vertical(1, Front);                       /*竖直中部对齐 1 */
        Align_horizontal(Left);                             /*移动：左对其*/
        sprintf(string, "原料区");                           /*状态更新*/
        lv_label_set_text(lv_obj_Transport.State.label, string);
        /*物料颜色检测*/
        Transport_Obiect_Detection_All();                   /*全部颜色检测*/
        /*物料抓取*/
        Align_Mid_horizontal(4, Left);                      /*移动至物料区中间*/
        Align_Mid_vertical(3, Front);                       /*竖直中部对齐 3 */
        for(uint8_t i=0; i<3; i++)
        {
            Transport_Obiect_Catch(i);
        }
        /* 任务结束 */
        Transport_Stop();
    }
}

void Transport_Init()
{
    Transport.Point_Car[0].x = 60; 
    Transport.Point_Car[0].y = 200; 
    Transport.Point_Car[0].z = 50; 

    
    Transport.Point_Car[1].x = 60; 
    Transport.Point_Car[1].y = 160; 
    Transport.Point_Car[1].z = 50; 
    
    Transport.Point_Car[2].x = 0; 
    Transport.Point_Car[2].y = 190; 
    Transport.Point_Car[2].z = 50; 
}