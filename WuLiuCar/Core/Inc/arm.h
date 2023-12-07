#ifndef arm_h
#define arm_h
#include "main.h"
#include <key.h>
#include <display.h>
#include <math.h>

#define ARM_ADD_SUB_1 30
#define ARM_ADD_SUB_2 20
#define ARM_ADD_SUB_3 15

typedef struct{
	float value;
	float increment;
	int min;
	int max;
}value_;

typedef struct{
	value_ x;
	value_ y;
	value_ z;
	float a;
	float b;
	float c;
	float d;
}arm_;

void Control_NUM(mouse_information_ mouse_information, uint8_t page); 
void Control_Angle(mouse_information_ mouse_information, uint8_t page);
void Control_Angle_ble(uint8_t change);

void Control_NUM_(volatile uint32_t* arm_p, uint8_t mode, int min, int max);
void Control_NUM_add_sub(volatile uint32_t* arm_p, uint8_t key, uint8_t mode, int max_min, int sign);
//void Control_NUM_add(volatile uint32_t* arm_p, uint8_t* key, uint8_t mode, int max);
//void Control_NUM_sub(volatile uint32_t* arm_p, uint8_t* key, uint8_t mode, int min);

void Control_Angle_(value_* v1, value_* v2, value_* v3, uint8_t mode);
void Control_Angle_add_sub(value_* v1, value_* v2, value_* v3, uint8_t key, uint8_t mode,void (*p)(value_* v1, value_* v2, value_* v3, int step, int delay));
void Control_Angle_add_(value_* v1, value_* v2, value_* v3, int step, int delay);
void Control_Angle_sub_(value_* v1, value_* v2, value_* v3, int step, int delay);
void Angle_move_(volatile uint32_t* arm_p, float angle, int incremental);
void Angle_calculate(void);
void Angle_move(void);
void arm_open(void);
void arm_close(void);
void arm_Init(void);
#endif
