#include <ST7789V.h>

void SPI1_Init_16bit(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

void LCD_Writ_Bus(uint8_t data) 
{
   HAL_SPI_Transmit(&hspi1,&data,1,0xff);
}

void LCD_Writ_Bus_16bit(uint16_t data) 
{
   HAL_SPI_Transmit(&hspi1,(uint8_t*)&data,1,0xff);
}

void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_H;//写数据
	LCD_CS_L;
	LCD_Writ_Bus(dat);
	LCD_CS_H;
}

void LCD_WR_DATA_16bit(uint16_t dat)
{
	LCD_DC_H;//写数据
	LCD_CS_L;
	LCD_Writ_Bus_16bit(dat);
	LCD_CS_H;
}
 
void LCD_WR_CMD(uint8_t dat)
{
	LCD_DC_L;//写命令
	LCD_CS_L;
	LCD_Writ_Bus(dat);
	LCD_CS_H;
}

void LCD_WR_CMD_16bit(uint16_t dat)
{
	LCD_DC_L;//写命令
	LCD_CS_L;
	LCD_Writ_Bus_16bit(dat);
	LCD_CS_H;
}

void Address_set_16bit(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	LCD_WR_CMD_16bit(0x2a);
	LCD_WR_DATA_16bit(x1);
	LCD_WR_DATA_16bit(x2);

	LCD_WR_CMD_16bit(0x2b);
	LCD_WR_DATA_16bit(y1);
	LCD_WR_DATA_16bit(y2);

	LCD_WR_CMD_16bit(0x002C);					 						 
}

void LCD_Clear_16bit(uint16_t Color)
{
	uint16_t i,j;  	
	Address_set_16bit(0,0,LCD_W-1,LCD_H-1);
	for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{
			LCD_WR_DATA_16bit(Color);	 			 
		}
	}
}

void LCD_Reset()
{
    LCD_RST_H;    
    HAL_Delay(1);  			
    LCD_RST_L;  
    HAL_Delay(10);
    LCD_RST_H;    
    HAL_Delay(120); 			
}

void LCD_Init()
{ 
	LCD_Reset();
	LCD_WR_CMD(0x36); 
	LCD_WR_DATA8(0xA0); /*00:正常；60顺时针90°；A0:逆时针90°*/

	LCD_WR_CMD(0x3A); 
	LCD_WR_DATA8(0x05);

	LCD_WR_CMD(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_CMD(0xB7); 
	LCD_WR_DATA8(0x35);  

	LCD_WR_CMD(0xBB);
	LCD_WR_DATA8(0x19);

	LCD_WR_CMD(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_CMD(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_CMD(0xC3);
	LCD_WR_DATA8(0x12);   

	LCD_WR_CMD(0xC4);
	LCD_WR_DATA8(0x20);  

	LCD_WR_CMD(0xC6); 
	LCD_WR_DATA8(0x0F);    

	LCD_WR_CMD(0xD0); 
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_CMD(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_CMD(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_CMD(0x21); 

	LCD_WR_CMD(0x11); 
	HAL_Delay(120); 

	LCD_WR_CMD(0x29);
	
	SPI1_Init_16bit();
	LCD_Clear_16bit(BLACK);
}