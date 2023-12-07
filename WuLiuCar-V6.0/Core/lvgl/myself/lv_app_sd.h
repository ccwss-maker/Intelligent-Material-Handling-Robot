#ifndef LV_APP_SD_H
#define LV_APP_SD_H
#include "fatfs.h"
#include "lv_app_function.h"
typedef struct{
	lv_obj_t* label;
}lv_obj_SD_;

void SD_create(lv_obj_t * parent);

extern lv_obj_SD_ lv_obj_SD;
#endif
