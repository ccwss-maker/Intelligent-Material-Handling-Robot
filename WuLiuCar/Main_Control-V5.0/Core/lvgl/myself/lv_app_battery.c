#include "lv_app_battery.h"
#include "Master_Slave_Communication.h"

lv_obj_Battery_ lv_obj_Battery;

void Battery_create(lv_obj_t * parent)
{
	lv_obj_t * cont;
    lv_obj_t * section;

    /*第一块*/
    lv_menu_separator_create(parent);
    section = lv_menu_section_create(parent);
	/*电压*/
	cont = create_bar(section, NULL, NULL, "电压", &my_style_font_DingTalk_JinBuTi_Chinese_16, 0, 100, 0);
	lv_obj_Battery.label_voltage = lv_obj_get_child(cont, 1);
	lv_label_set_text(lv_obj_Battery.label_voltage, "0V");
	lv_obj_Battery.label_percent = lv_obj_get_child(cont, 2);
	lv_label_set_text(lv_obj_Battery.label_percent, "0%");
	lv_obj_Battery.bar_percent = lv_obj_get_child(cont, 3);
}

void Battery_Display()
{
	
		float V = (int)(10.0 * Sensor_Msg.Battery_Voltage + 0.5) / 10.0;
		int Percentage = (int)((V-8.7)/(12.5-8.7)*100);
		char string[10]={0};
		sprintf(string, "%.1fV",V);
		lv_label_set_text(lv_obj_Battery.label_voltage, string);
		sprintf(string,"%d%%",Percentage);
		lv_label_set_text(lv_obj_Battery.label_percent, string);
		lv_bar_set_value(lv_obj_Battery.bar_percent, Percentage, LV_ANIM_ON);
}