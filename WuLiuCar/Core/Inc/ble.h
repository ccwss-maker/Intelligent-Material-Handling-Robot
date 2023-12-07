#ifndef ble_h
#define ble_h
#include <usart.h>

typedef struct{
	uint8_t function;
	int16_t num;
	uint8_t check;
}message_;

typedef struct{
	int L_X;
	int L_Y;
	int L_3;
	int L_2;
	int L_1;
	int R_X;
	int R_Y;
	int R_3;
	int R_2;
	int R_1;
	int up;
	int down;
	int left;
	int right;
	int A;
	int B;
	int X;
	int Y;
	int none;
}key_;

void BLE_Receive(void);
void BLE_control(void);
void BLE_arm_control(void);
#endif
