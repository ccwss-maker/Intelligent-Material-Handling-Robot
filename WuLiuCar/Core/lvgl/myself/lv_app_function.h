#ifndef LV_APP_FUNCTION_H
#define LV_APP_FUNCTION_H
#include "lvgl.h"
#include "stdio.h"
#include "MY_SYMBOL.h"

lv_obj_t * create_text(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p);
lv_obj_t *  create_text_two(lv_obj_t * parent, const char * title, lv_style_t* style_font_title_p, char * txt, lv_style_t* style_font_txt_p);
lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p);
lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                         int32_t min, int32_t max,int32_t val);
lv_obj_t * create_slider_two(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                         int32_t min, int32_t max, float val);
lv_obj_t * create_btn(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p);
lv_obj_t * create_bar(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                         int32_t min, int32_t max,int32_t val);
lv_obj_t * create_spinbox(lv_obj_t * parent, const char * icon, lv_style_t* style_pic_p, const char * txt, lv_style_t* style_font_p,
                          uint8_t digit_count, uint8_t separator_position, int32_t i);

void lv_app_function_init();

extern lv_style_t my_style_pic;
extern lv_style_t my_style_font_DingTalk_JinBuTi_Chinese_16;  
extern lv_style_t my_style_font_DingTalk_JinBuTi_English_14;  

#endif
