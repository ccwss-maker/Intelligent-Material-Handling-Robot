#include "lv_app_motro_control.h"
#include "Motro_Control.h"

lv_obj_Motro_Control_ lv_obj_Motro_Control;

static void switch_event_feedback(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    if(lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
        Motro_Control.feedback_sign = true;
    }
    else
    {
        Motro_Control.feedback_sign = false;
    }
}

static void btn_feedback_rst_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        lv_obj_Motro_Control.Control.times = 0;
        lv_obj_Motro_Control.Control_Success_Only_Once.times = 0;
        lv_obj_Motro_Control.Control_Error_Only_Once.times = 0;
        lv_obj_Motro_Control.Control_Timeout.times = 0;
        lv_obj_Motro_Control.UART_Queue.times = 0;
    }
}

static void slider_event_cb_feedback(lv_event_t * e)
{
    char buf[20];
    int slider_value = (int)lv_slider_get_value(lv_obj_Motro_Control.slider_error.slider);
    lv_snprintf(buf, sizeof(buf), "%d", slider_value);
    lv_label_set_text(lv_obj_Motro_Control.slider_error.slider_label, buf);
    lv_obj_align_to(lv_obj_Motro_Control.slider_error.slider_label, lv_obj_Motro_Control.slider_error.slider, LV_ALIGN_CENTER, 0, -25);
    Motro_Control.feedback_error = slider_value;
}

void Motro_Control_create(lv_obj_t * parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*反馈控制开关*/
    cont= create_switch(section, NULL, NULL, "通讯反馈", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_Motro_Control.sw_open = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_Motro_Control.sw_open,70);
    lv_obj_add_event_cb(lv_obj_Motro_Control.sw_open, switch_event_feedback, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_state(lv_obj_Motro_Control.sw_open, LV_STATE_CHECKED);
    /*验证错误重新发送次数滑杆*/
    cont = create_slider(section, NULL, NULL, "反馈错误上限", &my_style_font_DingTalk_JinBuTi_Chinese_16, 1, 10, Motro_Control.feedback_error);
    lv_obj_Motro_Control.slider_error.slider = lv_obj_get_child(cont, 2);
    lv_obj_add_event_cb(lv_obj_Motro_Control.slider_error.slider, slider_event_cb_feedback, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_Motro_Control.slider_error.slider_label = lv_obj_get_child(cont, 1);
    /*第二块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*反馈计数*/
    cont = create_btn(section, NULL, NULL, "反馈统计", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_Motro_Control.btn_rst = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_Motro_Control.btn_rst,70);
    lv_obj_t* btn_feedback_rst_label = lv_label_create(lv_obj_Motro_Control.btn_rst);	
    lv_obj_add_style(btn_feedback_rst_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_feedback_rst_label, "复位");
    lv_obj_align(btn_feedback_rst_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_Motro_Control.btn_rst, btn_feedback_rst_event_handler,LV_EVENT_ALL,NULL);
    
    cont = lv_menu_cont_create(section);
    lv_obj_Motro_Control.Control.label = create_text_two(cont, "指令下发次数", &my_style_font_DingTalk_JinBuTi_Chinese_16, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_Motro_Control.Control_Success_Only_Once.label = create_text_two(cont, "指令单次通讯完成", &my_style_font_DingTalk_JinBuTi_Chinese_16, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_Motro_Control.Control_Error_Only_Once.label = create_text_two(cont, "指令单次通讯失败", &my_style_font_DingTalk_JinBuTi_Chinese_16, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_Motro_Control.Control_Timeout.label = create_text_two(cont, "指令通讯超时", &my_style_font_DingTalk_JinBuTi_Chinese_16, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    
    lv_obj_Motro_Control.UART_Queue.label = create_text_two(cont, "缓存队列大小", &my_style_font_DingTalk_JinBuTi_Chinese_16, "0", &my_style_font_DingTalk_JinBuTi_English_14);
}

void Motro_Control_Text()
{
    char data[20];

    sprintf(data, "%d", lv_obj_Motro_Control.Control.times);
    lv_label_set_text(lv_obj_Motro_Control.Control.label, data);
    sprintf(data, "%d", lv_obj_Motro_Control.Control_Success_Only_Once.times);
    lv_label_set_text(lv_obj_Motro_Control.Control_Success_Only_Once.label, data);
    sprintf(data, "%d", lv_obj_Motro_Control.Control_Error_Only_Once.times);
    lv_label_set_text(lv_obj_Motro_Control.Control_Error_Only_Once.label, data);
    sprintf(data, "%d", lv_obj_Motro_Control.Control_Timeout.times);
    lv_label_set_text(lv_obj_Motro_Control.Control_Timeout.label, data);

    lv_obj_Motro_Control.UART_Queue.times = Motro_Msg_Queue.size;
    sprintf(data, "%d", lv_obj_Motro_Control.UART_Queue.times);
    lv_label_set_text(lv_obj_Motro_Control.UART_Queue.label, data);
}