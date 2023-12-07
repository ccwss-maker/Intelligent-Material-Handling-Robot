#ifndef LV_APP_BTTERY_H
#define LV_APP_BTTERY_H
#include "lv_app_function.h"
typedef struct{
	lv_obj_t* bar_percent;
	lv_obj_t* label_voltage;
	lv_obj_t* label_percent;
}lv_obj_Battery_;

extern lv_obj_Battery_ lv_obj_Battery;
void Battery_create(lv_obj_t * parent);
void Battery_Display();

#endif
