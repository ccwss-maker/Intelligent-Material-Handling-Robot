#ifndef key_h
#define key_h
#include <gpio.h>
#include <main.h>
#include <tim.h>
#include <string.h>

#define KEY_TL HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_RESET
#define KEY_L HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)==GPIO_PIN_RESET
#define KEY_BL HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)==GPIO_PIN_RESET
#define KEY_T HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)==GPIO_PIN_RESET
#define KEY_C HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==GPIO_PIN_RESET
#define KEY_B HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==GPIO_PIN_RESET
#define KEY_TR HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==GPIO_PIN_RESET
#define KEY_R HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==GPIO_PIN_RESET
#define KEY_BR HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==GPIO_PIN_RESET

typedef struct{
	bool sign[2];
	/*0			1			2			3*/
	/*无		单次  慢    快*/
	/*是否按下标志*/
	/*单次按下读取标志*/
	uint8_t TL[2];
	uint8_t L[2];
	uint8_t BL[2];
	uint8_t T[2];
	uint8_t C[2];
	uint8_t B[2];
	uint8_t TR[2];
	uint8_t R[2];
	uint8_t BR[2];
}key_pressed_;

void key_test(void);
void GPIO_test(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t key[],bool* sign);
	
#endif
