#ifndef LV_APP_ARM_H
#define LV_APP_ARM_H
#include "lv_app_function.h"
#include "lv_app_wheel.h"

typedef struct{
    lv_obj_t *btn_rst;
    lv_obj_t *btn_Claw;
    lv_obj_t *label_Angle_X;
    lv_obj_t *label_Angle_Y;
    lv_obj_t *label_Angle_Z;
    lv_obj_t *label_Label_X;
    lv_obj_t *label_Label_Y;
    lv_obj_t *label_Label_Z;
    lv_obj_t *Symbol_X;
    lv_obj_t *Symbol_Y;
    lv_obj_t *Symbol_Z;
    slider_kid_ slider_X;
    slider_kid_ slider_Y;
    slider_kid_ slider_Z;
}Arm_Motor_Control_;

typedef struct{
    lv_obj_t *btn_rst;
    lv_obj_t *btn_set_0;
    lv_obj_t *label_Angle_Rear_Arm;
    lv_obj_t *label_Angle_Front_Arm;
    lv_obj_t *label_Angle_Pedestal;
    lv_obj_t *label_Label_Rear_Arm;
    lv_obj_t *label_Label_Front_Arm;
    lv_obj_t *label_Label_Pedestal;
    lv_obj_t *Symbol_Rear_Arm;
    lv_obj_t *Symbol_Front_Arm;
    lv_obj_t *Symbol_Pedestal;
    slider_kid_ slider_Rear_Arm;
    slider_kid_ slider_Front_Arm;
    slider_kid_ slider_Pedestal;
}Arm_Motor_Debug_;

typedef struct{
    lv_obj_t *sw_debug;
    lv_obj_t *sw_power;
    Arm_Motor_Control_ Motor_Control;
    Arm_Motor_Debug_ Motor_Debug;
}lv_obj_ARM_;

void lv_Arm_Coord_Control();
void lv_Arm_Coord_Control_Reset();
void ARM_create(lv_obj_t *parent);
void Arm_Symbol_Refresh();

extern lv_obj_ARM_ lv_obj_ARM;
#endif
