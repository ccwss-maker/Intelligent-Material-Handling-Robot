#include "Battery.h"
#include "adc.h"
#include "Master_Slave_Communication.h"

void Battery_Detection_Init()
{
	HAL_ADC_Start(&hadc1);
}

void Battery_Detection()
{
	HAL_ADC_PollForConversion(&hadc1, 10);
	uint16_t ADC = HAL_ADC_GetValue(&hadc1);
	Sensor_Msg.Battery_Voltage = (float)ADC / 4096.0f * 3.3f * 6.1f;
}
