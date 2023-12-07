#ifndef LV_APP_PID_H
#define LV_APP_PID_H
#include "lv_app_function.h"
#include "lv_app_imu.h"
#include "lv_app_motro_control.h"

typedef struct{
    lv_obj_t * Chart;
	chart_kid_ ref;
	chart_kid_ set;
}chart_;

typedef struct{
    lv_obj_t* Add;
    lv_obj_t* Sub;
}spinbox_button_;

typedef struct{
    lv_obj_t* spinbox;
    spinbox_button_ button;
}spinbox_PID_kid_;

typedef struct{
    spinbox_PID_kid_ P;
    spinbox_PID_kid_ I;
    spinbox_PID_kid_ D;
    spinbox_PID_kid_ max_out;
    spinbox_PID_kid_ max_iout;
    spinbox_PID_kid_ Integral_Separation_Threshold;
    spinbox_PID_kid_ Dead_Zone;
}spinbox_PID_;

typedef struct{
    lv_obj_t* sw_pid_debug;
    lv_obj_t* sw_ble_control;
    lv_obj_t* btn_clear;
    lv_obj_t* btn_save;
    lv_obj_t* btn_clear_label;
    lv_obj_t* btn_save_label;
    lv_obj_t* slider_value_set;
    spinbox_PID_ PID;
    chart_ Chart;
    uint8_t ID;
}lv_obj_PID_KID_;

typedef struct{
    lv_obj_t * sw_pid;
    Label_Kids_ Control_Frequence;
    lv_obj_PID_KID_ speed;
    lv_obj_PID_KID_ position;
}lv_obj_PID_;

void lv_app_pid_init();
void PID_create(lv_obj_t * parent);
void PID_Frequence_Detection();
void PID_Display();
void PID_Debug_Set_BLE(uint8_t ID_PID);

extern lv_obj_PID_ lv_obj_PID;

#endif
