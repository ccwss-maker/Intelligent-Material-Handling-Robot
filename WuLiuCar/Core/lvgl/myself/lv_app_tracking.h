#ifndef LV_APP_TRACKING_H
#define LV_APP_TRACKING_H
#include "lv_app_function.h"

typedef struct{
	lv_obj_t* Circle[8];
}Tracking_Kid_;

typedef struct{
	lv_obj_t* label;
    Tracking_Kid_ Tracking_Kid[4];
}lv_obj_Tracking_;

void Tracking_create(lv_obj_t * parent);
void Tracking_Draw();

extern lv_obj_Tracking_ lv_obj_Tracking;
#endif
