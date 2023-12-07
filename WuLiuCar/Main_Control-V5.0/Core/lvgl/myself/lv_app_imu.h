#ifndef LV_APP_IMU_H
#define LV_APP_IMU_H

#include "lv_app_function.h"

typedef struct{
	lv_chart_series_t* series;
	lv_obj_t* label;
}chart_kid_;

typedef struct{
	lv_obj_t* Chart_W;
	lv_obj_t* Chart_RPY;

	chart_kid_ Wx;
	chart_kid_ Wy;
	chart_kid_ Wz;
	chart_kid_ W;

	chart_kid_ Roll;
	chart_kid_ Pitch;
	chart_kid_ Yaw;

}lv_obj_IMU_;

extern lv_obj_IMU_ lv_obj_IMU;
void IMU_create(lv_obj_t * parent);
void IMU_Display();
#endif
