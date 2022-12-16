#include "segment_display.h"
#include "io.h"
#include "math.h"

static char digits_dictionary[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7F, 0x6F }; // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
static char dot_symbol = 0x80;


static char digits_number(int value)
{
	value = abs(value);

#if DIGITS_NUMBER >= 7 || DIGITS_NUMBER < 1
	#error "DIGITS_NUMBER can`t be more than 6"
#endif
#if DIGITS_NUMBER >= 6
	if(value >= 100000)
		return 6;
#endif
#if DIGITS_NUMBER >= 5
	if(value >= 10000)
		return 5;
#endif
#if DIGITS_NUMBER >= 4
	if(value >= 1000)
		return 4;
#endif
#if DIGITS_NUMBER >= 3
	if(value >= 100)
		return 3;
#endif
#if DIGITS_NUMBER >= 2
	if(value >= 10)
		return 2;
#endif

	return 1;
}

static void display_clear(Display_t* displayPtr)
{
	int i;
	for(i=0;i<DIGITS_NUMBER;i++)
		displayPtr->symbols[i] = 0b00000000;
}

static void display_print_number(Display_t* displayPtr, short number, char position)
{
	do
	{
		char digit = number % 10;
		displayPtr->symbols[position] = digits_dictionary[digit];
		if(position < DIGITS_NUMBER)
			position++;
		number /= 10;
	}
	while(number > 0);
}

void display_print_float(Display_t* displayPtr, float value)
{
	int i = 0;
	
	int integer_part = (int)value;
	char decimal_part[DIGITS_NUMBER-1];
	char point_precision = 0;

#ifdef DYNAMIC_POINT
	point_precision = DIGITS_NUMBER - digits_number(integer_part);
	integer_part = float_to_digits(value, point_precision, decimal_part);
	point_precision = DIGITS_NUMBER - digits_number(integer_part);
#else
	point_precision = STATIC_POINT_MAX_PRECISION;
	integer_part = float_to_digits(value, point_precision, decimal_part);
	point_precision = min(DIGITS_NUMBER - digits_number(integer_part), STATIC_POINT_MAX_PRECISION);
#endif
	
	display_clear(displayPtr);
	
	display_print_number(displayPtr, integer_part, point_precision);
	
	if(point_precision > 0)
	{
		displayPtr->symbols[point_precision] |= dot_symbol;
		
		for(i=0;i<point_precision;i++)
			display_print_number(displayPtr, decimal_part[point_precision-1-i], i);
	}
}

char display_init(Display_t* displayPtr, Display_IOs_t ios)
{
	displayPtr->ios = ios;
	displayPtr->current_symbol = 0;
	display_clear(displayPtr);
}

void display_update(Display_t* displayPtr)
{
	int i=0;
	
	char current_symbol = displayPtr->current_symbol;
	
	display_disable(displayPtr);

	for(i=0;i<SEGMENTS_NUMBER;i++)
		if(displayPtr->symbols[current_symbol] & (1 << i))
			IO_SetLine(displayPtr->ios.segments[i], HIGH);
		else
			IO_SetLine(displayPtr->ios.segments[i], LOW);

	IO_SetLine(displayPtr->ios.digits[current_symbol], LOW);

	if(current_symbol == (DIGITS_NUMBER-1))
		displayPtr->current_symbol = 0;
	else
		displayPtr->current_symbol++;
}

void display_disable(Display_t* displayPtr)
{
	int i=0;
	
	for(i=0;i<DIGITS_NUMBER;i++)
		IO_SetLine(displayPtr->ios.digits[i], HIGH);
}