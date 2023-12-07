#ifndef TRACKING_H
#define TRACKING_H
#include <main.h>
#include <i2c.h>

typedef struct{
	uint8_t data[4];
	uint8_t Slaver_Address;
}Tracking_;

void Tracking_Transmit();
void Tracking_Receive();
void Tracking_ErrorCallback(I2C_HandleTypeDef *hi2c);

extern Tracking_ Tracking;

#endif

