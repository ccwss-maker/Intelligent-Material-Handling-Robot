#include <GT911.h>

/*创建触摸结构体*/
Touch_Struct	User_Touch;

uint8_t GT911_WriteReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen)
{
	return HAL_I2C_Mem_Write(&hi2c1, GT911_DIV_W, _usRegAddr, I2C_MEMADD_SIZE_16BIT, _pRegBuf, _ucLen, HAL_MAX_DELAY);
}

uint8_t GT911_ReadReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen)
{
	return HAL_I2C_Mem_Read(&hi2c1, GT911_DIV_R, _usRegAddr, I2C_MEMADD_SIZE_16BIT, _pRegBuf, _ucLen, HAL_MAX_DELAY);
}

void Address_SET_0xBA()
{
	TOUCH_RST_L;
	TOUCH_INT_L;
	HAL_Delay(1);
	TOUCH_RST_H;
	HAL_Delay(6);
}

/*
	功能：软件复位gt911
	参数1：gt_SR_type（为1时开始软件复位,为0时结束软件复位）
*/
uint8_t Software_Reset(uint8_t gt_SR_type)
{
	uint8_t _temp=0;	//中间变量
	if(gt_SR_type)
	{
		_temp=2;
		return GT911_WriteReg(GT_CTRL_REG, &_temp, 1);
	}
	else
	{
		_temp=0;
		return GT911_WriteReg(GT_CTRL_REG, &_temp, 1);
	}
}

void gt911_Scanf(void)
{	
	uint8_t _temp;	//中间变量
	
	GT911_ReadReg(GT_GSTID_REG, &_temp, 1);//读0x814E寄存器

	User_Touch.Touch_State = _temp;
	
	User_Touch.Touch_Number = (User_Touch.Touch_State & 0x0f);	//获取触摸点数
	User_Touch.Touch_State = (User_Touch.Touch_State & 0x80);	//触摸状态
	
	switch(User_Touch.Touch_State)	//判断是否有触摸数据
	{
		case TOUCH__NO:		//没有数据
			
		break;
		case TOUCH_ING:		//触摸中~后，有数据，并读出数据
			
		for(uint8_t i=0; i<User_Touch.Touch_Number; i++)
		{
			GT911_ReadReg((GT_TPD_Sta + i*8 + X_L), &_temp, 1);	//读出触摸x坐标的低8位
			User_Touch.Touch_XY[i].X_Point  = _temp;
			GT911_ReadReg((GT_TPD_Sta + i*8 + X_H), &_temp, 1);	//读出触摸x坐标的高8位
			User_Touch.Touch_XY[i].X_Point |= (_temp<<8);
			
			GT911_ReadReg((GT_TPD_Sta + i*8 + Y_L), &_temp, 1);	//读出触摸y坐标的低8位
			User_Touch.Touch_XY[i].Y_Point  = _temp;
			GT911_ReadReg((GT_TPD_Sta + i*8 + Y_H), &_temp, 1);	//读出触摸y坐标的高8位
			User_Touch.Touch_XY[i].Y_Point |= (_temp<<8);
			
			/*逆时针旋转90°*/
			uint16_t data = User_Touch.Touch_XY[i].X_Point;
			User_Touch.Touch_XY[i].X_Point = 320 - User_Touch.Touch_XY[i].Y_Point;
			User_Touch.Touch_XY[i].Y_Point = data;

			GT911_ReadReg((GT_TPD_Sta + i*8 + S_L), &_temp, 1);	//读出触摸大小数据的低8位
			User_Touch.Touch_XY[i].S_Point  = _temp;
			GT911_ReadReg((GT_TPD_Sta + i*8 + S_H), &_temp, 1);	//读出触摸大小数据的高8位
			User_Touch.Touch_XY[i].S_Point |= (_temp<<8);
		}
		_temp=0;
		GT911_WriteReg(GT_GSTID_REG, &_temp, 1);	//清除数据标志位
		break;
	}
}

void TOUCH_INIT()
{
	TOUCH_RST_H;
	HAL_Delay(50);
	TOUCH_RST_L;
	HAL_Delay(50);

	Address_SET_0xBA();
	Software_Reset(1);
	HAL_Delay(10);
	Software_Reset(0);
}