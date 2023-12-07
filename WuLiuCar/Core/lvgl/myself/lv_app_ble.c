#include "lv_app_ble.h"
#include "stdio.h"
#include "usart.h"
lv_obj_BLE_ lv_obj_BLE;

lv_obj_t * create_text_2(lv_obj_t * parent, const char * txt_1, const char * txt_2, lv_style_t* style_font_p)
{
    lv_obj_t * obj = lv_menu_cont_create(parent);

    lv_obj_t * label_1 = NULL;
    lv_obj_t * label_2 = NULL;

    if(txt_1) 
    {
        label_1 = lv_label_create(obj);
        if(style_font_p)
        {
            lv_obj_add_style(label_1,style_font_p,LV_STATE_DEFAULT);
        }
        lv_label_set_text(label_1, txt_1);
        lv_obj_set_flex_grow(label_1, 1);
    }

    if(txt_2) 
    {
        label_2 = lv_label_create(obj);
        if(style_font_p)
        {
            lv_obj_add_style(label_2,style_font_p,LV_STATE_DEFAULT);
        }
        lv_label_set_text(label_2, txt_2);
    }

    return obj;
}

lv_obj_t * create_text_3(lv_obj_t * parent, const char * txt_1, const char * txt_2, lv_style_t* style_font_p)
{
    lv_obj_t * label_1 = NULL;
    lv_obj_t * label_2 = NULL;
    if(txt_1)
    {
        label_1 = lv_label_create(parent);
        if(style_font_p)
        {
            lv_obj_add_style(label_1,style_font_p,LV_STATE_DEFAULT);
        }
        lv_label_set_text(label_1,  txt_1);
        lv_obj_set_flex_grow(label_1, 1);
        lv_obj_add_flag(label_1, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }
    if(txt_2)
    {
        label_2 = lv_label_create(parent);
        if(style_font_p)
        {
            lv_obj_add_style(label_2,style_font_p,LV_STATE_DEFAULT);
        }
        lv_label_set_text(label_2, txt_2);
    }
    return label_2;
}

void BLE_create(lv_obj_t * parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    if(HAL_GPIO_ReadPin(BLE_STATE_GPIO_Port,BLE_STATE_Pin)==GPIO_PIN_SET)
    {
        cont = create_text(section, NULL, NULL, "手柄未连接", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    }
    else
    {
        cont = create_text(section, NULL, NULL, "手柄已连接", &my_style_font_DingTalk_JinBuTi_Chinese_16);
    }
    lv_obj_BLE.lable.BLE = lv_obj_get_child(cont, 0);
    /*第二块*/
    lv_menu_separator_create(parent);/*分隔符*/
    section = lv_menu_section_create(parent);

    lv_obj_t * obj = lv_menu_cont_create(section);
    lv_obj_BLE.lable.digital_data = create_text_3(obj,"digital_data", "0x00", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.L_X = create_text_3(obj,"L_X", "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.L_Y = create_text_3(obj,"L_Y", "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.L_2 = create_text_3(obj,"L_2", "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.R_X = create_text_3(obj,"R_X", "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.R_Y = create_text_3(obj,"R_Y", "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.R_2 = create_text_3(obj,"R_2", "0", &my_style_font_DingTalk_JinBuTi_English_14);
    
    // cont = create_text_2(section, "digital_data","0x00", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.digital_data = lv_obj_get_child(cont, 1);
    // cont = create_text_2(section, "L_X","0000", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.L_X = lv_obj_get_child(cont, 1);
    // cont = create_text_2(section, "L_Y","0000", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.L_Y = lv_obj_get_child(cont, 1);
    // cont = create_text_2(section, "L_2","0000", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.L_2 = lv_obj_get_child(cont, 1);
    // cont = create_text_2(section, "R_X","0000", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.R_X = lv_obj_get_child(cont, 1);
    // cont = create_text_2(section, "R_Y","0000", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.R_Y = lv_obj_get_child(cont, 1);
    // cont = create_text_2(section, "R_2","0000", &my_style_font_DingTalk_JinBuTi_English_14);
    // lv_obj_BLE.lable.R_2 = lv_obj_get_child(cont, 1);
}