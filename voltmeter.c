#include "voltmeter.h"
#include "config.h"

float voltmeter_get_voltage(ADC1_Channel_TypeDef adc_channel)
{
	short adc_value = 0;
	
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
             adc_channel,
             ADC1_PRESSEL_FCPU_D12, 
             ADC1_EXTTRIG_GPIO, 
             DISABLE, 
             ADC1_ALIGN_RIGHT, 
             ADC1_SCHMITTTRIG_CHANNEL0, 
             DISABLE);
	
	ADC1_ClearFlag(ADC1_FLAG_EOC);
	ADC1_StartConversion();
	while( !ADC1_GetFlagStatus( ADC1_FLAG_EOC));
	adc_value = ADC1_GetConversionValue();
	
	if(adc_value != 0)
		return (VOLTMETER_DIVIDER * ((ADC_RESOLUTION * adc_value) + ADC_OFFSET_ERROR));
	
	return 0;
}