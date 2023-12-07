#include "lv_app_battery.h"

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