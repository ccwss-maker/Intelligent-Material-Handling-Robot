#ifndef LV_APP_WHEEL_H
#define LV_APP_WHEEL_H
#include "lv_app_function.h"

typedef struct{
    lv_obj_t * slider;
    lv_obj_t * slider_label;
}slider_kid_;

typedef struct{
    lv_obj_t * sw_power;            /*动力控制*/
    lv_obj_t * btn_rst;             /*转速复位*/
    slider_kid_ slider_FL;
    slider_kid_ slider_FR;
    slider_kid_ slider_BL;
    slider_kid_ slider_BR;
}lv_obj_WHEEL_;

void WHEEL_create(lv_obj_t * parent);
void Wheel_slider_clear();
void lv_obj_WHEEL_Control(bool state_sw, bool state_slider);

extern lv_obj_WHEEL_ lv_obj_WHEEL;
#endif
