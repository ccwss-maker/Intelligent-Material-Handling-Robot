#ifndef ble_h
#define ble_h
#include <main.h>
#include <usart.h>

typedef struct{
	bool Control_Sign;
	int x;
	int y;
	int z;
}BLE_Arm_;

void BLE_control(void);
void BLE_Arm_Control(void);
void BLE_Arm_Init(void);
extern BLE_Arm_ BLE_Arm;
#endif
