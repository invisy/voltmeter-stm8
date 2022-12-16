#ifndef _SEGMENT_DISPLAY_H_
	#define _SEGMENT_DISPLAY_H_
	
	#include "io.h"
	
	//1-6 digits
	#define DIGITS_NUMBER 3
	#define SEGMENTS_NUMBER 8
	
	#define DYNAMIC_POINT
	
#ifndef DYNAMIC_POINT
	#define STATIC_POINT_PRECISION 1 	// n = 1. One decimal point.
#endif
	
	typedef struct
	{
		IOLine_TypeDef segments[SEGMENTS_NUMBER];
		IOLine_TypeDef digits[DIGITS_NUMBER];
	} Display_IOs_t;
	
	typedef struct
	{
		Display_IOs_t ios;
		char symbols[DIGITS_NUMBER];
		char current_symbol;
	} Display_t;
	
	char display_init(Display_t* displayPtr, Display_IOs_t ios);
	void display_print_float(Display_t* displayPtr, float value);
	void display_update(Display_t* displayPtr);					//Run once for a 3-10ms
	void display_disable(Display_t* displayPtr);				//Use when displays use the same lines
#endif
