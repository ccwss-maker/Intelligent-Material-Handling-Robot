#include "Battery.h"
#include "adc.h"
#include "stdio.h"
#include "lv_app.h"

Battery_ Battery={true};

void Battery_Test()
{
	if(Battery.Tese_Sign)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,10);      
		if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
		{
			char string[10]={0};
			uint16_t AD_Value = HAL_ADC_GetValue(&hadc1);
			float V = 20.13f/4096*AD_Value;
			sprintf(string, "%.1fV",V);
			lv_label_set_text(lv_obj_Battery.label_voltage, string);
			sprintf(string,"%d%%",(int)((V-8.7)/(12.5-8.7)*100));
			lv_label_set_text(lv_obj_Battery.label_percent, string);
			lv_bar_set_value(lv_obj_Battery.bar_percent, (int)((V-8.7)/(12.5-8.7)*100), LV_ANIM_OFF);
		}
		Battery.Tese_Sign = false;
	}
}