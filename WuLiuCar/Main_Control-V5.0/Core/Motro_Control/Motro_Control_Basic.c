#include "Motro_Control_Basic.h"
#include "Motro_Control_Wheel.h"
#include "Motro_Control_Arm.h"
#include "wheel.h"
#include "stdlib.h" //abs
#include "math.h"   //fabs

uint8_t XOR_Calculate(uint8_t data[] , uint8_t length)
{
	uint8_t result = data[0];
	for(uint8_t i = 1; i < length; i++)
	{
		result ^= data[i];
	}
	return result;
}

void Motor_Open(UART_HandleTypeDef *huart, uint8_t Address)
{
	uint8_t data[4]={Address,0xF3,0X01};
	data[3] = XOR_Calculate(data, 3);
	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);
	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 4, receive_verify);
	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 4, receive_verify);
	
}

void Motor_Close(UART_HandleTypeDef *huart, uint8_t Address)
{
	uint8_t data[4]={Address,0xF3,0X00};
	data[3] = XOR_Calculate(data, 3);
	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);
	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 4, receive_verify);
	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 4, receive_verify);
}

void Motor_MStep_Set(UART_HandleTypeDef *huart, uint8_t Address, uint8_t num)  //设置细分步数
{
	uint8_t data[4]={Address, 0x84, num};
	data[3] = XOR_Calculate(data, 3);
	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);
	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 4, receive_verify);
	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 4, receive_verify);
}

void Motor_Set_0(UART_HandleTypeDef *huart, uint8_t Address)
{
	uint8_t data[4]={Address, 0x0A, 0x6D,};
	data[3] = XOR_Calculate(data, 3);
	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);
	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 4, receive_verify);
	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 4, receive_verify);
}

/*速度控制*/
void Motor_Speed_Control(UART_HandleTypeDef *huart, uint8_t Address, int16_t speed, uint8_t aacelerated)/*1为正方向，-1为反方向*/
{
	int sign=1;
	if(Address%2==0)	sign=-1;
	uint8_t data[6]={Address, 0xF6, 0x00, 0x00, aacelerated};
	if(sign*speed>0)	data[2]=0x10;
	else				data[2]=0x00;
	data[2] |= abs(speed)>>8;
	data[3] = abs(speed);
	data[5] = XOR_Calculate(data, 5);

	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);

	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 6, receive_verify);
	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 6, receive_verify);
}

/*相对位置控制*/
void Motor_Relative_Position_Control(UART_HandleTypeDef *huart, uint8_t Address, int8_t direction, uint16_t speed, uint8_t accelerated, float angle, uint16_t Subdivision_Step, float Reduction_Ratio)
{
	uint8_t data[9]={Address,0xFD,0X00,0X00,accelerated,0X00,0X00,0X00};
	if(direction * angle >0)	data[2] = 0x10;
	else						data[2] = 0x00;					
	data[2] |= speed>>8;
	data[3] = speed;
	angle = fabs(angle);
	uint32_t pulse = (uint32_t)(angle * Subdivision_Step * Reduction_Ratio / 1.8f);
	data[5] = (uint8_t)(pulse >> 16);
	data[6] = (uint8_t)(pulse >> 8);
	data[7] = (uint8_t)(pulse);
	data[8] = XOR_Calculate(data, 8);
	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);

	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 9, receive_verify);
 	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 9, receive_verify);
}

void Motor_Relative_Position_Stop(UART_HandleTypeDef *huart, uint8_t Address)
{
	uint8_t data[9]={Address,0xFD,0X12,0XFF,0xFF,0X00,0X00,0X00};
	data[8] = XOR_Calculate(data, 8);
	uint8_t receive_verify[3]={Address,0x02};
	if(Address==0)	receive_verify[0]=1;
	receive_verify[2] = XOR_Calculate(receive_verify, 2);

	if(huart == UART_Wheel)		Motro_Transmit_Verify_Wheel(huart, data, 9, receive_verify);
 	else if(huart == UART_Arm)	Motro_Transmit_Verify_Arm(huart, data, 9, receive_verify);
}

void Motro_Control_Init()
{
	Motro_Control_Wheel.feedback_sign = false;
	Motro_Control_Wheel.feedback_error = 5;
	
	Motro_Control_Arm.feedback_sign = true;
	Motro_Control_Arm.feedback_error = 5;
	
}