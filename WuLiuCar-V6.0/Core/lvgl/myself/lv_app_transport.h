#ifndef LV_APP_Transport_H
#define LV_APP_Transport_H
#include "lv_app_function.h"

typedef struct{
    lv_obj_t *  label;
    char string[10];
}Transport_Label_Kids_;

typedef struct{
    lv_obj_t* btn_start;
    Transport_Label_Kids_ State;
	Transport_Label_Kids_ Camera_Object_Order;
    Transport_Label_Kids_ Camera_Object_Detect;
}lv_obj_Transport_;

void Transport_create(lv_obj_t * parent);
void Camera_Msg_Display();

extern lv_obj_Transport_ lv_obj_Transport;
#endif
