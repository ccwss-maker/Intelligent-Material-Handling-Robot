#include "lv_app_wheel.h"
#include "wheel.h"
#include "stdio.h"

lv_obj_WHEEL_ lv_obj_WHEEL;

void lv_obj_WHEEL_Control(bool state_sw, bool state_slider)
{
    if(state_sw)
    {
        lv_obj_clear_state(lv_obj_WHEEL.sw_power,  LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(lv_obj_WHEEL.sw_power, LV_STATE_DISABLED);
    }

    if(state_slider)
    {
        lv_obj_clear_state(lv_obj_WHEEL.btn_rst, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_WHEEL.slider_FL.slider, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_WHEEL.slider_FR.slider, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_WHEEL.slider_BL.slider, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_WHEEL.slider_BR.slider, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(lv_obj_WHEEL.btn_rst, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_WHEEL.slider_FL.slider, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_WHEEL.slider_FR.slider, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_WHEEL.slider_BL.slider, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_WHEEL.slider_BR.slider, LV_STATE_DISABLED);
    }
}
void Wheel_slider_clear()
{
    Wheel_Speed_Control(Wheel_Addr_All, 0, 255);

    char buf[20];
    lv_slider_set_value(lv_obj_WHEEL.slider_FL.slider, 0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_WHEEL.slider_FR.slider, 0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_WHEEL.slider_BL.slider, 0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_WHEEL.slider_BR.slider, 0, LV_ANIM_ON);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_WHEEL.slider_FL.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_WHEEL.slider_FR.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_WHEEL.slider_BL.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_WHEEL.slider_BR.slider_label, buf);
}

static void switch_event_cb_power(lv_event_t * e)
{
    if(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED))
    {
        Wheel_Open();
        lv_obj_WHEEL_Control(true, true);
    }
    else
    {
        Wheel_Close();
        Wheel_slider_clear();
        lv_obj_WHEEL_Control(true, false);
    }
}

static void switch_event_cb_debug(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    if(lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
        Wheel.debug_sign=true;
        lv_obj_WHEEL_Control(true,false);
    }
    else
    {
        Wheel.debug_sign=false;
        Wheel_slider_clear();
        lv_obj_clear_state(lv_obj_WHEEL.sw_power,LV_STATE_CHECKED);
        lv_obj_WHEEL_Control(false,false);
        Wheel_Close();
    }
}

static void btn_rst_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        Wheel_slider_clear();
    }
}

static void Slider_Event_Cb_Speed_Control(lv_event_t * e)
{
    char buf[20];
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * obj_label = lv_obj_get_child(obj->parent, 1);
    uint8_t Addr = (uint8_t)e->user_data;
    int slider_value = (int)lv_slider_get_value(obj);
    lv_snprintf(buf, sizeof(buf), "%d", slider_value);
    lv_label_set_text(obj_label, buf);
    Wheel_Speed_Control(Addr,slider_value,255);
}

void WHEEL_create(lv_obj_t * parent)
{	
    lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    section = lv_menu_section_create(parent);
    /*调试总开关*/
    cont = create_switch(section, NULL, NULL, "开启调试", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_t * sw_debug = lv_obj_get_child(cont, 1);
    lv_obj_set_width(sw_debug,70);
    lv_obj_add_event_cb(sw_debug, switch_event_cb_debug, LV_EVENT_VALUE_CHANGED, NULL);
    /*动力开关*/
    cont = create_switch(section, NULL, NULL, "动力控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_WHEEL.sw_power = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_WHEEL.sw_power,70);
    lv_obj_add_event_cb(lv_obj_WHEEL.sw_power, switch_event_cb_power, LV_EVENT_VALUE_CHANGED, NULL);
    
    /*第二块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*复位*/
    cont = create_btn(section, NULL, NULL, "转速控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_WHEEL.btn_rst = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_WHEEL.btn_rst,70);
    lv_obj_t* btn_rst_label = lv_label_create(lv_obj_WHEEL.btn_rst);	
    lv_obj_add_style(btn_rst_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_rst_label, "复位");
    lv_obj_align(btn_rst_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_WHEEL.btn_rst, btn_rst_event_handler,LV_EVENT_ALL,NULL);
    /*FL轮滑杆*/
    cont = create_slider(section, NULL, NULL, "左前", &my_style_font_DingTalk_JinBuTi_Chinese_16, -40, 40, 0);
    lv_obj_WHEEL.slider_FL.slider = lv_obj_get_child(cont, 2);
    lv_obj_add_event_cb(lv_obj_WHEEL.slider_FL.slider, Slider_Event_Cb_Speed_Control, LV_EVENT_VALUE_CHANGED, Wheel_Addr_FL);
    lv_obj_WHEEL.slider_FL.slider_label = lv_obj_get_child(cont, 1);
    /*FR轮滑杆*/
    cont = create_slider(section, NULL, NULL, "右前", &my_style_font_DingTalk_JinBuTi_Chinese_16, -40, 40, 0);
    lv_obj_WHEEL.slider_FR.slider = lv_obj_get_child(cont, 2);
    lv_obj_add_event_cb(lv_obj_WHEEL.slider_FR.slider, Slider_Event_Cb_Speed_Control, LV_EVENT_VALUE_CHANGED, Wheel_Addr_FR);
    lv_obj_WHEEL.slider_FR.slider_label = lv_obj_get_child(cont, 1);
    /*BL轮滑杆*/
    cont = create_slider(section, NULL, NULL, "左后", &my_style_font_DingTalk_JinBuTi_Chinese_16, -40, 40, 0);
    lv_obj_WHEEL.slider_BL.slider = lv_obj_get_child(cont, 2);
    lv_obj_add_event_cb(lv_obj_WHEEL.slider_BL.slider, Slider_Event_Cb_Speed_Control, LV_EVENT_VALUE_CHANGED, Wheel_Addr_BL);
    lv_obj_WHEEL.slider_BL.slider_label = lv_obj_get_child(cont, 1);
    /*BR轮滑杆*/
    cont = create_slider(section, NULL, NULL, "右后", &my_style_font_DingTalk_JinBuTi_Chinese_16, -40, 40, 0);
    lv_obj_WHEEL.slider_BR.slider = lv_obj_get_child(cont, 2);
    lv_obj_add_event_cb(lv_obj_WHEEL.slider_BR.slider, Slider_Event_Cb_Speed_Control, LV_EVENT_VALUE_CHANGED, Wheel_Addr_BR);
    lv_obj_WHEEL.slider_BR.slider_label = lv_obj_get_child(cont, 1);

    lv_obj_WHEEL_Control(false,false);
}