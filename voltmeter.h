#ifndef _VOLTMETER_H_
	#define _VOLTMETER_H_

	#include "stm8s.h"

	float voltmeter_get_voltage(ADC1_Channel_TypeDef adc_channel);
#endif
