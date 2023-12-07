#ifndef __ST7789V_H
#define __ST7789V_H	
#include "main.h"
#include "spi.h"

#define LCD_H 320
#define LCD_W 240

#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define YELLOW 0xffe0
#define WHITE  0xffff
#define BLACK  0x0000
#define PURPLE 0xf81f

#define LCD_DC_H HAL_GPIO_WritePin(OLED_SPI2_DC_GPIO_Port,OLED_SPI2_DC_Pin,GPIO_PIN_SET)
#define LCD_DC_L HAL_GPIO_WritePin(OLED_SPI2_DC_GPIO_Port,OLED_SPI2_DC_Pin,GPIO_PIN_RESET)

#define LCD_RST_H HAL_GPIO_WritePin(OLED_SPI2_RES_GPIO_Port,OLED_SPI2_RES_Pin,GPIO_PIN_SET)
#define LCD_RST_L HAL_GPIO_WritePin(OLED_SPI2_RES_GPIO_Port,OLED_SPI2_RES_Pin,GPIO_PIN_RESET)

#define LCD_CS_H HAL_GPIO_WritePin(OLED_SPI2_CS_GPIO_Port,OLED_SPI2_CS_Pin,GPIO_PIN_SET)
#define LCD_CS_L HAL_GPIO_WritePin(OLED_SPI2_CS_GPIO_Port,OLED_SPI2_CS_Pin,GPIO_PIN_RESET)

void SPI2_Init_16bit(void);
void LCD_Init();
void Address_set_16bit(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA_16bit(uint16_t dat);
void LCD_Clear_16bit(uint16_t Color);
#endif  
	 
	 



