#include "lv_app_ble.h"
#include "stdio.h"
#include "usart.h"
#include "Master_Slave_Communication.h"
lv_obj_BLE_ lv_obj_BLE;

void BLE_create(lv_obj_t * parent)
{
    lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
    if(Sensor_Msg.BLE_Data.Connected_Sign != true)
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
    lv_obj_BLE.lable.digital_data = create_text_two(obj,"digital_data", &my_style_font_DingTalk_JinBuTi_English_14, "0x00", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.L_X = create_text_two(obj,"L_X", &my_style_font_DingTalk_JinBuTi_English_14, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.L_Y = create_text_two(obj,"L_Y", &my_style_font_DingTalk_JinBuTi_English_14, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.L_2 = create_text_two(obj,"L_2", &my_style_font_DingTalk_JinBuTi_English_14, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.R_X = create_text_two(obj,"R_X", &my_style_font_DingTalk_JinBuTi_English_14, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.R_Y = create_text_two(obj,"R_Y", &my_style_font_DingTalk_JinBuTi_English_14, "0", &my_style_font_DingTalk_JinBuTi_English_14);
    lv_obj_BLE.lable.R_2 = create_text_two(obj,"R_2", &my_style_font_DingTalk_JinBuTi_English_14, "0", &my_style_font_DingTalk_JinBuTi_English_14);
}

void BLE_Display()
{
	if(Sensor_Msg.BLE_Data.Connected_Sign != true)
		lv_label_set_text(lv_obj_BLE.lable.BLE,"手柄未连接");
	else
		lv_label_set_text(lv_obj_BLE.lable.BLE,"手柄已连接");
		
	char data[10]={0};
    sprintf(data,"0x%02x",Sensor_Msg.BLE_Data.digital_data);
	lv_label_set_text(lv_obj_BLE.lable.digital_data,data);
    sprintf(data,"%d",Sensor_Msg.BLE_Data.L_X);
	lv_label_set_text(lv_obj_BLE.lable.L_X,data);
    sprintf(data,"%d",Sensor_Msg.BLE_Data.L_Y);
	lv_label_set_text(lv_obj_BLE.lable.L_Y,data);
    sprintf(data,"%d",Sensor_Msg.BLE_Data.L_2);
	lv_label_set_text(lv_obj_BLE.lable.L_2,data);
    sprintf(data,"%d",Sensor_Msg.BLE_Data.R_X);
	lv_label_set_text(lv_obj_BLE.lable.R_X,data);
    sprintf(data,"%d",Sensor_Msg.BLE_Data.R_Y);
	lv_label_set_text(lv_obj_BLE.lable.R_Y,data);
    sprintf(data,"%d",Sensor_Msg.BLE_Data.R_2);
	lv_label_set_text(lv_obj_BLE.lable.R_2,data);
}