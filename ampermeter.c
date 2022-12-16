#include "ampermeter.h"
#include "config.h"

float ampermeter_get_current(ADC1_Channel_TypeDef adc_channel)
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
	{
		float clear_voltage = ((ADC_RESOLUTION * adc_value) - AMPERMETER_OFFSET);
		
		if(clear_voltage >= 0)
			return (clear_voltage/AMPERMETER_AMPLIFIER/AMPERMETER_SHUNT_R);
	}
	
	return 0;
}