#ifndef display_h
#define display_h
#include <main.h>
#include "u8g2.h"
#include "stdio.h"

typedef struct{
	bool selected;
	uint8_t mouse_1;
	uint8_t mouse_2[2];
}mouse_information_;
	
	void OLED_display(void);

//	void display(int num,int x,int y,int sign,char * __restrict, ... );

	void page_1_init(void);
	void page_1_update_all(void);
	void page_1_update_num(void);

	void page_2_init(void);
	void page_2_update_all(void);
	void page_2_update_num(void);

	void page_3_init(void);
	void page_3_update_all(void);
	void page_3_update_num(void);
	
//	void page_4_init();
//	void page_4_update_all(key_pressed_ *key_pressed_p);
//	void page_4_update_num();
	
	void display_str(char* s,int x,int y,int sign);
	void mouse_test(mouse_information_* mouse_information, uint8_t x, uint8_t y);
	void update_mouse_page(void);
	void update_mouse_1(mouse_information_* mouse_information);
	void update_mouse_2(mouse_information_* mouse_information, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint8_t y2);
	uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr);
#endif
