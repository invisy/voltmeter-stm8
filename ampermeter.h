#ifndef _AMPERMETER_H_
	#define _AMPERMETER_H_
	
	#include "stm8s.h"

	float ampermeter_get_current(ADC1_Channel_TypeDef adc_channel);
#endif
