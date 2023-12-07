#include "lv_app_transport.h"
#include "Transport.h"
#include "Camera.h"
#include "stdio.h"

lv_obj_Transport_ lv_obj_Transport;

static void btn_start_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        lv_obj_t * obj = lv_event_get_target(e);
        lv_obj_t * label = lv_obj_get_child(obj, 0);
        if(lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            Transport.sign_Running = true;
            lv_label_set_text(label, "停止");
        }
        else
        {
            Transport.sign_Running = false;
            Transport_Stop();
            lv_label_set_text(label, "启动");
        }
    }
}

void Transport_create(lv_obj_t * parent)
{
	lv_obj_t * cont;
    lv_obj_t * section;
    lv_obj_t * menu = lv_obj_get_parent(lv_obj_get_parent(parent));
    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*启动*/
    cont = lv_menu_cont_create(section);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER);
    lv_obj_Transport.btn_start = lv_btn_create(cont);
    lv_obj_add_flag(lv_obj_Transport.btn_start, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_t * btn_start_label = lv_label_create(lv_obj_Transport.btn_start);
    lv_obj_add_style(btn_start_label,&my_style_font_DingTalk_JinBuTi_Chinese_16,LV_STATE_DEFAULT);
    lv_label_set_text(btn_start_label, "启动");
    lv_obj_align(btn_start_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(lv_obj_Transport.btn_start, btn_start_event_handler,LV_EVENT_ALL,NULL);
    lv_obj_set_size(lv_obj_Transport.btn_start, 100, 62);
    /*第二块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    /*信息*/
    cont = lv_menu_cont_create(section);
    lv_obj_Transport.State.label = create_text_two(cont,"搬运进度", &my_style_font_DingTalk_JinBuTi_Chinese_16, "起点", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_Transport.Camera_Object_Order.label = create_text_two(cont,"搬运顺序", &my_style_font_DingTalk_JinBuTi_Chinese_16, "无无无", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    lv_obj_Transport.Camera_Object_Detect.label = create_text_two(cont,"原料顺序", &my_style_font_DingTalk_JinBuTi_Chinese_16, "无无无", &my_style_font_DingTalk_JinBuTi_Chinese_16);
}

void Camera_Msg_Display()
{
	char string[10]={0};
    for(uint8_t i=0; i<3; i++)
    {
        switch(Transport.Camera_Object_Order[i])
        {
            case Object_Red:
                sprintf((string+3*i), "红");
                break;
            case Object_Green:
                sprintf((string+3*i), "绿");
                break;
            case Object_Blue:
                sprintf((string+3*i), "蓝");
                break;
            default:
                sprintf((string+3*i), "无");
        }
    }
    lv_label_set_text(lv_obj_Transport.Camera_Object_Order.label, string);

    memset(string, 0, sizeof(string));
    for(uint8_t i=0; i<3; i++)
    {
        switch(Transport.Camera_Object_Detect[i])
        {
            case Object_Red:
                sprintf(string+3*i, "红");
                break;
            case Object_Green:
                sprintf(string+3*i, "绿");
                break;
            case Object_Blue:
                sprintf(string+3*i, "蓝");
                break;
            default:
                sprintf(string+3*i, "无");
        }
    }
    lv_label_set_text(lv_obj_Transport.Camera_Object_Detect.label, string);
}