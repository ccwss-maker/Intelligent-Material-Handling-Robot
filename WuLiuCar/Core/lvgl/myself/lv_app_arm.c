#include "lv_app_arm.h"
#include "arm.h"
#include "stdio.h"

lv_obj_ARM_ lv_obj_ARM;

static lv_obj_t * lv_menu_page_create_end_control(lv_obj_t *parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * sub_page = lv_menu_page_create(parent, NULL);
    lv_obj_set_style_pad_hor(sub_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    /*第一块*/
    section = lv_menu_section_create(sub_page);

    return sub_page;
}

/*机械臂电机调试部分*/
void Arm_slider_clear()
{
    // Motor_Speed_Control(0,0,255);

    char buf[20];
    lv_slider_set_value(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider, 0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider, 0, LV_ANIM_ON);
    lv_slider_set_value(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider, 0, LV_ANIM_ON);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider_label, buf);

    lv_snprintf(buf, sizeof(buf), "%d", 0);
    lv_label_set_text(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider_label, buf);
}

void lv_obj_Arm_Control(bool state_sw, bool state_slider)
{
    if(state_sw)
    {
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.sw_power,  LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.btn_set_0,  LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.sw_power, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.btn_set_0,  LV_STATE_DISABLED);
    }

    if(state_slider)
    {
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.btn_rst, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider, LV_STATE_DISABLED);
        lv_obj_clear_state(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider, LV_STATE_DISABLED);
    }
    else
    {
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.btn_rst, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider, LV_STATE_DISABLED);
        lv_obj_add_state(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider, LV_STATE_DISABLED);
    }
}

static void switch_event_cb_debug(lv_event_t * e)
{
   lv_obj_t * obj = lv_event_get_target(e);
   if(lv_obj_has_state(obj, LV_STATE_CHECKED))
   {
       Arm.debug_sign=true;
       lv_obj_Arm_Control(true,false);
   }
   else
   {
       Arm.debug_sign=false;
       Arm_slider_clear();
       lv_obj_clear_state(lv_obj_ARM.Motor_Debug.sw_power,LV_STATE_CHECKED);
       lv_obj_Arm_Control(false,false);
       Arm_Close();
   }
}

static void switch_event_cb_power(lv_event_t * e)
{
   if(lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED))
   {
       Arm_Open();
       lv_obj_Arm_Control(true, true);
   }
   else
   {
       Arm_Close();
       Arm_slider_clear();
       lv_obj_Arm_Control(true, false);
   }
}
static void btn_set_0_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        Arm_Set_0();
    }
}
static void btn_rst_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) 
    {
        Arm_slider_clear();
        Arm_Position_Control(5, Clockwise, Arm_Velocity, Arm_Accelerate, 0, true);
        Arm_Position_Control(6, Clockwise, Arm_Velocity, Arm_Accelerate, 0, true);
        Arm_Position_Control(7, Clockwise, Arm_Velocity, Arm_Accelerate, 0, true);
    }
}

static void slider_event_cb_Arm_Debug(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(lv_obj_get_parent(obj), 3);
    uint8_t address = (uint8_t)lv_obj_get_user_data(obj);
    char buf[20];
    int slider_value = (int)lv_slider_get_value(obj);
    lv_snprintf(buf, sizeof(buf), "%d", slider_value);
    lv_label_set_text(label, buf);
    Arm_Position_Control(address, Clockwise, Arm_Velocity, Arm_Accelerate, slider_value, true);
}

static lv_obj_t * lv_menu_page_create_motor_debug(lv_obj_t *parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * sub_page = lv_menu_page_create(parent, NULL);
    lv_obj_set_style_pad_hor(sub_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(parent), 0), 0);
    /*第一块*/
    section = lv_menu_section_create(sub_page);
    /*调试总开关*/
    cont = create_switch(section, NULL, NULL, "开启调试", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.Motor_Debug.sw_debug = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.Motor_Debug.sw_debug,70);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.sw_debug, switch_event_cb_debug, LV_EVENT_VALUE_CHANGED, NULL);
    /*动力开关*/
    cont = create_switch(section, NULL, NULL, "动力控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.Motor_Debug.sw_power = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.Motor_Debug.sw_power,70);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.sw_power, switch_event_cb_power, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_state(lv_obj_ARM.Motor_Debug.sw_power,  LV_STATE_DISABLED);
    /*第二块*/
    lv_menu_separator_create(sub_page);
    section = lv_menu_section_create(sub_page);
    /*零点设置*/
    cont = create_btn(section, NULL, NULL, "零点设置", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.Motor_Debug.btn_set_0 = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.Motor_Debug.btn_set_0,70);
    lv_obj_t* btn_set_0_label = lv_label_create(lv_obj_ARM.Motor_Debug.btn_set_0);	
    lv_obj_add_style(btn_set_0_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_set_0_label, "执行");
    lv_obj_align(btn_set_0_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.btn_set_0, btn_set_0_event_handler,LV_EVENT_ALL,NULL);
    lv_obj_add_state(lv_obj_ARM.Motor_Debug.btn_set_0,  LV_STATE_DISABLED);
    /*复位*/
    cont = create_btn(section, NULL, NULL, "角度控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_ARM.Motor_Debug.btn_rst = lv_obj_get_child(cont, 1);
    lv_obj_set_width(lv_obj_ARM.Motor_Debug.btn_rst,70);
    lv_obj_t* btn_rst_label = lv_label_create(lv_obj_ARM.Motor_Debug.btn_rst);	
    lv_obj_add_style(btn_rst_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_rst_label, "复位");
    lv_obj_align(btn_rst_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.btn_rst, btn_rst_event_handler,LV_EVENT_ALL,NULL);
    lv_obj_add_state(lv_obj_ARM.Motor_Debug.btn_rst,  LV_STATE_DISABLED);
    /*大臂滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "大臂", &my_style_font_DingTalk_JinBuTi_Chinese_16, 0, 80, 0);
    lv_obj_ARM.Motor_Debug.label_Label_Rear_Arm = lv_obj_get_child(cont, 0);
    lv_obj_ARM.Motor_Debug.Symbol_Rear_Arm = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Debug.label_Angle_Rear_Arm = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider, slider_event_cb_Arm_Debug, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider_label = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider->user_data = (void*)5;
    lv_obj_add_state(lv_obj_ARM.Motor_Debug.slider_Rear_Arm.slider,  LV_STATE_DISABLED);
    /*小臂滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "小臂", &my_style_font_DingTalk_JinBuTi_Chinese_16, 0, 80, 0);
    lv_obj_ARM.Motor_Debug.label_Label_Front_Arm = lv_obj_get_child(cont, 0);
    lv_obj_ARM.Motor_Debug.Symbol_Front_Arm = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Debug.label_Angle_Front_Arm = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider, slider_event_cb_Arm_Debug, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider_label = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider->user_data = (void*)6;
    lv_obj_add_state(lv_obj_ARM.Motor_Debug.slider_Front_Arm.slider,  LV_STATE_DISABLED);
    /*底座滑杆*/
    cont = create_slider_two(section, USR_SYMBOL_Complete, &my_style_pic, "底座", &my_style_font_DingTalk_JinBuTi_Chinese_16, -180, 180, 0);
    lv_obj_ARM.Motor_Debug.label_Label_Pedestal = lv_obj_get_child(cont, 0);
    lv_obj_ARM.Motor_Debug.Symbol_Pedestal = lv_obj_get_child(cont, 1);
    lv_obj_ARM.Motor_Debug.label_Angle_Pedestal = lv_obj_get_child(cont, 2);
    lv_obj_ARM.Motor_Debug.slider_Pedestal.slider = lv_obj_get_child(cont, 4);
    lv_obj_add_event_cb(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider, slider_event_cb_Arm_Debug, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_ARM.Motor_Debug.slider_Pedestal.slider_label = lv_obj_get_child(cont, 3);
    lv_obj_ARM.Motor_Debug.slider_Pedestal.slider->user_data = (void*)7;
    lv_obj_add_state(lv_obj_ARM.Motor_Debug.slider_Pedestal.slider,  LV_STATE_DISABLED);
    return sub_page;
}

void ARM_create(lv_obj_t * parent)
{	
    lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * menu = lv_obj_get_parent(lv_obj_get_parent(parent));
    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    lv_obj_t * label;
    /*末端控制*/
    lv_obj_t * sub_page_end_control = lv_menu_page_create_end_control(menu);
    label = create_text(section, NULL, &my_style_pic, "末端控制", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, label, sub_page_end_control);
    /*电机调试*/
    lv_obj_t * sub_page_motor_debug = lv_menu_page_create_motor_debug(menu);
    label = create_text(section, NULL, &my_style_pic, "电机调试", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_menu_set_load_page_event(menu, label, sub_page_motor_debug);
}

void Arm_Symbol_Set(uint8_t Address, const void *src)
{
	lv_obj_t ** pic = &lv_obj_ARM.Motor_Debug.Symbol_Rear_Arm;
    switch (Address)
	{
		case Arm_Rear_Address:
			pic += 0;
			break;
		case Arm_Front_Address:
			pic += 1;
			break;
		case Arm_Pedestal_Address:
			pic += 2;
			break;
	}
	lv_img_set_src(*pic, src);
}