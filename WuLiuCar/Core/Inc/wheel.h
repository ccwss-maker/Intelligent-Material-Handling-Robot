#ifndef wheel_h
#define wheel_h
#include <main.h>
#include <display.h>
#include "stdio.h"
#include "PID.h"
#define PID_ADD_SUB_1 1
#define PID_ADD_SUB_2 2
#define PID_ADD_SUB_3 1

#define Car_a 0.1315
#define Car_b	0.084
#define Wheel_r 0.046

//	void Control_ALL(key_pressed_* key_pressed_p, mouse_information_ mouse_information, uint8_t page);
void Control_SPD(mouse_information_ mouse_information, uint8_t page);
void Control_Mecanum(float v_x, float v_y, float w, uint8_t aacelerated);

//	void Control(int* wheel_p, key_pressed_* key_pressed_p, uint8_t mode, int min, uint16_t max);
//	void Control_add(int* wheel_p, uint8_t* key, uint8_t mode, uint16_t max);
//	void Control_sub(int* wheel_p, uint8_t* key, uint8_t mode, int min);
void Control_implement(void);

void Control(float* wheel_p, uint8_t mode, int min, int max);
void Control_add(float* wheel_p, uint8_t key, uint8_t mode, int max);
void Control_sub(float* wheel_p, uint8_t key, uint8_t mode, int min);
//	void Speed_PID();
void Speed_pub(uint8_t address, int16_t speed, uint8_t aacelerated);

void wheel_close(void);
void wheel_open(void);
void Wheel_Init(void);
void Wheel_PID_BLE(void);
#endif
