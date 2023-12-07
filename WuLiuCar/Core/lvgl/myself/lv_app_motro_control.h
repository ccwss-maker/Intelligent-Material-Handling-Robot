#ifndef LV_APP_MOTRO_CONTROL_H
#define LV_APP_MOTRO_CONTROL_H
#include "lv_app_function.h"
#include "lv_app_wheel.h"

typedef struct{
    lv_obj_t *  label;
    uint32_t times;
}Label_Kids_;

typedef struct{
    lv_obj_t * sw_open;                         /*通讯反馈控制*/
    lv_obj_t * btn_rst;                         /*数据统计清零*/
    slider_kid_ slider_error;
    Label_Kids_ Control;                     /*指令下发次数*/
    Label_Kids_ Control_Success_Only_Once;   /*指令单次通讯完成次数*/                       
    Label_Kids_ Control_Error_Only_Once;     /*指令单次通讯未完成次数*/                  
    Label_Kids_ Control_Timeout;             /*指令通讯超时*/
    Label_Kids_ UART_Queue;                  /*缓存队列大小*/
}lv_obj_Motro_Control_;

void Motro_Control_create(lv_obj_t * parent);
void Motro_Control_Text();

extern lv_obj_Motro_Control_ lv_obj_Motro_Control;
#endif
