#ifndef ble_h
#define ble_h
#include <main.h>
#include <usart.h>

#define ble_uart_buffer 36

typedef struct{
	volatile uint8_t rx_len;  
	volatile uint8_t recv_end_flag; 
	uint8_t receive[ble_uart_buffer];
}ble_uart_;

typedef struct{
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
	
	uint8_t head;
	int16_t digital_data;/*L_3;L_1;R_3;R_1;up;down;left;right;A;B;X;Y;*/
	int16_t L_X;
	int16_t L_Y;
	int16_t L_2;
	int16_t R_X;
	int16_t R_Y;
	int16_t R_2;
	int16_t check;/*如果值最终为0，说明没有按键按*/
	
	bool break_sign;
}ble_data_;

typedef struct{
	bool Control_Sign;
	int x;
	int y;
	int z;
}BLE_Arm_;

void BLE_Receive();
void BLE_control(void);
void BLE_State_Display();
void BLE_Arm_Control(void);
void BLE_Init();
extern ble_data_ ble_data;
extern ble_uart_ ble_uart;
extern BLE_Arm_ BLE_Arm;
#endif
