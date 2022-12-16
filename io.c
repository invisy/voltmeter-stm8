#include "io.h"

const GPIO_Line_TypeDef IOs[NUM_IO] =
{
	#define X_IO(a,b,c,d,e) {b,c,d,e},
	IO_TABLE
	#undef X_IO
};

void IO_Init()
{
	char i = 0;
	
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	
	for(i=0; i<NUM_IO; i++)
	{
		IO_ConfigLine(i, IOs[i].mode);
		IO_SetLine(i, IOs[i].state);
	}
}

void IO_ConfigLine(IOLine_TypeDef line, GPIO_Mode_TypeDef mode)
{
	GPIO_Init(IOs[line].port, IOs[line].pin, mode);
}

void IO_SetLine(IOLine_TypeDef line, GPIO_State_TypeDef state)
{
	if(state == LOW)
		GPIO_WriteLow(IOs[line].port, IOs[line].pin);
	else if (state == HIGH)
		GPIO_WriteHigh(IOs[line].port, IOs[line].pin);
	else
		GPIO_WriteReverse(IOs[line].port, IOs[line].pin);
}