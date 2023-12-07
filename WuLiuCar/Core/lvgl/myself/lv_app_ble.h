#ifndef LV_APP_BLE_H
#define LV_APP_BLE_H
#include "fatfs.h"
#include "lv_app_function.h"

typedef struct{
	lv_obj_t* BLE;
	lv_obj_t* digital_data;
	lv_obj_t* L_X;
	lv_obj_t* L_Y;
	lv_obj_t* L_2;
	lv_obj_t* R_X;
	lv_obj_t* R_Y;
	lv_obj_t* R_2;
}lable_;

typedef struct{
	lable_ lable;
}lv_obj_BLE_;

void BLE_create(lv_obj_t * parent);
void USART6_UART_Init(uint16_t BaudRate);
extern lv_obj_BLE_ lv_obj_BLE;
#endif
