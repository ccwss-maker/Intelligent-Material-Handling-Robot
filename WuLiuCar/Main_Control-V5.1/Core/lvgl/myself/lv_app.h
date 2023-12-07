#ifndef LV_APP_H
#define LV_APP_H
#include "lv_app_function.h"
#include "lv_port_disp.h"
#include "lv_app_imu.h"
#include "lv_app_sd.h"
#include "lv_app_ble.h"
#include "lv_app_wheel.h"
#include "lv_app_arm.h"
#include "lv_app_pid.h"
#include "lv_app_battery.h"
#include "lv_app_tracking.h"
#include "lv_app_motro_control.h"

typedef struct{
    bool Sport;
    bool Arm;
    bool Motro_Communication;
    bool BLE;
    bool IMU;
    bool Tracking;
    bool Battery;
}Page_Sign_;

void lv_demo_myself();
void LVGL_Display();

extern Page_Sign_ Page_Sign;

#endif
