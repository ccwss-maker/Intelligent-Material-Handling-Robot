#include "sdram.h"
#include "fmc.h"
 
/*
*********************************************************************************************************
*    �� �� ��: SDRAM_Send_Cmd
*    ����˵��: ��SDRAM��������
*    ��    ��: hsdram : SDRAM_HandleTypeDef �ṹ��
*              bankx ��0,��BANK5�����SDRAM����ָ�� �� 1,��BANK6�����SDRAM����ָ��
*              cmd ��ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
*             refresh:��ˢ�´���
*             regval:ģʽ�Ĵ����Ķ���  
*    �� �� ֵ: 0,����;1,ʧ��.
*********************************************************************************************************
*/
uint8_t SDRAM_Send_Cmd(SDRAM_HandleTypeDef *hsdram,uint8_t bankx,uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    uint32_t target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;
    
    if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;       
    else if(bankx==2) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;   
    Command.CommandMode=cmd;                //����
    Command.CommandTarget=target_bank;      //Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber=refresh;      //��ˢ�´���
    Command.ModeRegisterDefinition=regval;  //Ҫд��ģʽ�Ĵ�����ֵ
    if(HAL_SDRAM_SendCommand(hsdram,&Command,0X1000)==HAL_OK) //��SDRAM��������
    {
        return 0;  
    }
    else return 1;    
}
 
/*
*********************************************************************************************************
*    �� �� ��: SDRAM_Init
*    ����˵��: ����SDRAM��ʼ������
*    ��    ��: hsdram : SDRAM_HandleTypeDef �ṹ��              
*    �� �� ֵ: ��
*    ��    ע: ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��: COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
*            ����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=137.5Mhz,����Ϊ8192(2^13).    ����,COUNT=64*1000*137.5/8192-20=1055  
*********************************************************************************************************
*/
void SDRAM_Init(SDRAM_HandleTypeDef *hsdram)
{
    uint32_t temp=0;
    //SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //ʱ������ʹ��      
    HAL_Delay(1);                                  //������ʱ200us
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_PALL,1,0);       //�����д洢��Ԥ���   
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//������ˢ�´���   
    //����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
    //bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
    //bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
    temp=(uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |    //����ͻ������:1(������1/2/4/8)
              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |    //����ͻ������:����(����������/����)
              SDRAM_MODEREG_CAS_LATENCY_3           |    //����CASֵ:3(������2/3)
              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������
    SDRAM_Send_Cmd(hsdram,1,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //����SDRAM��ģʽ�Ĵ���    
    HAL_SDRAM_ProgramRefreshRate(hsdram,1055/*918*/);//����ˢ��Ƶ��
}
 
 
/*
*********************************************************************************************************
*    �� �� ��: FMC_SDRAM_WriteBuffer
*    ����˵��: ��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
*    ��    ��: pBuffer:�ֽ�ָ��
*             WriteAddr:Ҫд��ĵ�ַ 
*             n:Ҫд����ֽ���
*    �� �� ֵ: ��
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
*    �� �� ��: FMC_SDRAM_ReadBuffer
*    ����˵��: ��ָ����ַ((ReadAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
*    ��    ��: pBuffer:�ֽ�ָ��
*             WriteAddr:Ҫ��������ʼ��ַ 
*             n:Ҫ�������ֽ���
*    �� �� ֵ: ��
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

	/* дSDRAM */
	pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
	{
		*pSRAM++ = i;
	}

	/* ��SDRAM */
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

	/* ��SDRAM �������󷴲�д�� */
	pSRAM = (uint32_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < EXT_SDRAM_SIZE / 4; i++)
	{
		*pSRAM = ~*pSRAM;
		pSRAM++;
	}

	/* �ٴαȽ�SDRAM������ */
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

	/* ���԰��ֽڷ�ʽ����, Ŀ������֤ FSMC_NBL0 �� FSMC_NBL1 ���� */
	pBytes = (uint8_t *)EXT_SDRAM_ADDR;
	for (i = 0; i < sizeof(ByteBuf); i++)
	{
		*pBytes++ = ByteBuf[i];
	}

	/* �Ƚ�SDRAM������ */
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