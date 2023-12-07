#include "sdram.h"
#include "fmc.h"
 
/*
*********************************************************************************************************
*    函 数 名: SDRAM_Send_Cmd
*    功能说明: 向SDRAM发送命令
*    形    参: hsdram : SDRAM_HandleTypeDef 结构体
*              bankx ：0,向BANK5上面的SDRAM发送指令 ； 1,向BANK6上面的SDRAM发送指令
*              cmd ：指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
*             refresh:自刷新次数
*             regval:模式寄存器的定义  
*    返 回 值: 0,正常;1,失败.
*********************************************************************************************************
*/
uint8_t SDRAM_Send_Cmd(SDRAM_HandleTypeDef *hsdram,uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    uint32_t target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;
    
    if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;       
    else if(bankx==2) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;   
    Command.CommandMode=cmd;                //命令
    Command.CommandTarget=target_bank;      //目标SDRAM存储区域
    Command.AutoRefreshNumber=refresh;      //自刷新次数
    Command.ModeRegisterDefinition=regval;  //要写入模式寄存器的值
    if(HAL_SDRAM_SendCommand(hsdram,&Command,0X1000)==HAL_OK) //向SDRAM发送命令
    {
        return 0;  
    }
    else return 1;    
}
 
/*
*********************************************************************************************************
*    函 数 名: SDRAM_Init
*    功能说明: 发送SDRAM初始化序列
*    形    参: hsdram : SDRAM_HandleTypeDef 结构体              
*    返 回 值: 无
*    备    注: 刷新频率计数器(以SDCLK频率计数),计算方法: COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
*            我们使用的SDRAM刷新周期为64ms,SDCLK=137.5Mhz,行数为8192(2^13).    所以,COUNT=64*1000*137.5/8192-20=1055  
*********************************************************************************************************
*/
void SDRAM_Init(SDRAM_HandleTypeDef *hsdram)
{
    uint32_t temp=0;
    //SDRAM控制器初始化完成以后还需要按照如下顺序初始化SDRAM
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //时钟配置使能      
    HAL_Delay(1);                                  //至少延时200us
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_PALL,1,0);       //对所有存储区预充电   
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//设置自刷新次数   
    //配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
    //bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
    //bit9为指定的写突发模式，bit10和bit11位保留位
    temp=(uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |    //设置突发长度:1(可以是1/2/4/8)
              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |    //设置突发类型:连续(可以是连续/交错)
              SDRAM_MODEREG_CAS_LATENCY_3           |    //设置CAS值:3(可以是2/3)
              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //设置突发写模式:1,单点访问
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //设置SDRAM的模式寄存器    
    HAL_SDRAM_ProgramRefreshRate(hsdram,1055/*918*/);//设置刷新频率
}
 
 
/*
*********************************************************************************************************
*    函 数 名: FMC_SDRAM_WriteBuffer
*    功能说明: 在指定地址(WriteAddr+Bank5_SDRAM_ADDR)开始,连续写入n个字节.
*    形    参: pBuffer:字节指针
*             WriteAddr:要写入的地址 
*             n:要写入的字节数
*    返 回 值: 无
*********************************************************************************************************
*/
void FMC_SDRAM_WriteBuffer(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t n)
{
    for(;n!=0;n--)
    {
        *(__IO uint8_t *)(EXT_SDRAM_ADDR+WriteAddr)=*pBuffer;
        WriteAddr++;
        pBuffer++;
    }
}
 
/*
*********************************************************************************************************
*    函 数 名: FMC_SDRAM_ReadBuffer
*    功能说明: 在指定地址((ReadAddr+Bank5_SDRAM_ADDR))开始,连续读出n个字节.
*    形    参: pBuffer:字节指针
*             WriteAddr:要读出的起始地址 
*             n:要读出的字节数
*    返 回 值: 无
*********************************************************************************************************
*/
void FMC_SDRAM_ReadBuffer(uint8_t *pBuffer,uint32_t ReadAddr,uint32_t n)
{
    for(;n!=0;n--)
    {
        *pBuffer++=*(__IO uint8_t *)(EXT_SDRAM_ADDR+ReadAddr);
        ReadAddr++;
    }
}

uint32_t bsp_TestExtSDRAM(void)
{
	uint32_t i;
	uint32_t *pSRAM;
	uint8_t *pBytes;
	uint32_t err;
	const uint8_t ByteBuf[4] = {0x55, 0xA5, 0x5A, 0xAA};

	/* 写SDRAM */
	pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
	{
		*pSRAM++ = i;
	}

	/* 读SDRAM */
	err = 0;
	pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
	{
		if (*pSRAM++ != i)
		{
			err++;
		}
	}

	if (err >  0)
	{
		return  (4 * err);
	}

	/* 对SDRAM 的数据求反并写入 */
	pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
	{
		*pSRAM = ~*pSRAM;
		pSRAM++;
	}

	/* 再次比较SDRAM的数据 */
	err = 0;
	pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
	{
		if (*pSRAM++ != (~i))
		{
			err++;
		}
	}

	if (err >  0)
	{
		return (4 * err);
	}

	/* 测试按字节方式访问, 目的是验证 FSMC_NBL0 、 FSMC_NBL1 口线 */
	pBytes = (uint8_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		*pBytes++ = ByteBuf[i];
	}

	/* 比较SDRAM的数据 */
	err = 0;
	pBytes = (uint8_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		if (*pBytes++ != ByteBuf[i])
		{
			err++;
		}
	}
	if (err >  0)
	{
		return err;
	}
	return 0;
}