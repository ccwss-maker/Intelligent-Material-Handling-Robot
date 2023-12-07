#ifndef BLE_H
#define BLE_H
#include <main.h>
#include <usart.h>

#define BLE_Uart_Buffer 36

typedef struct{
	uint8_t head;
	int16_t digital_data;/*L_3;L_1;R_3;R_1;up;down;left;right;A;B;X;Y;*/
	int16_t L_X;
	int16_t L_Y;
	int16_t L_2;
	int16_t R_X;
	int16_t R_Y;
	int16_t R_2;
	int16_t check;/*如果值最终为0，说明没有按键按*/
}BLE_Pretreatment_Data_;

typedef struct{
	bool Connected_Sign;
	uint8_t L_3;
	uint8_t L_1;
	uint8_t R_3;
	uint8_t R_1;
	uint8_t up;
	uint8_t down;
	uint8_t left;
	uint8_t right;
	uint8_t A;
	uint8_t B;
	uint8_t X;
	uint8_t Y;

	int16_t digital_data;/*L_3;L_1;R_3;R_1;up;down;left;right;A;B;X;Y;*/
	int16_t L_X;
	int16_t L_Y;
	int16_t L_2;
	int16_t R_X;
	int16_t R_Y;
	int16_t R_2;
	int16_t check;/*如果值最终为0，说明没有按键按*/
}BLE_Data_;

void BLE_Init();
void BLE_Receive();
void BLE_Detection();
void BLE_Receive_Clear();
extern uint8_t BLE_Receive_Data[BLE_Uart_Buffer];
#endif
