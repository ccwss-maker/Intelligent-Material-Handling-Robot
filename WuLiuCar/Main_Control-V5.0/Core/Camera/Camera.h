#ifndef CAMERA_H
#define CAMERA_H
#include <main.h>

enum{
    Task_Null,
    Task_QR_Code_Detection,
    Task_Object_Detection
};

enum{
	Object_None,
	Object_Red,
	Object_Green,
	Object_Blue,
};

typedef struct{
    uint8_t task_sign;
    uint8_t Uart_Receive[10];
}Camera_;

#define UART_Camera &huart6
#define No_Msg  "No_Msg"
#define QR_Code "QR_Code"
#define Object  "Object"

void Camera_Msg_Clear();
void Camera_Receive_IT();
void Camera_QR_Code_Detection();
void Camera_Object_Detection();

extern Camera_ Camera;
#endif

