/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "io.h"
#include "segment_display.h"
#include "voltmeter.h"
#include "ampermeter.h"
#include "math.h"

/* Private defines -----------------------------------------------------------*/
static Display_t voltmeter_display;
static Display_t ampermeter_display;
/* Private function prototypes -----------------------------------------------*/
static void clock_init(void);
static void peripherals_init(void);
static void init_drivers(void);
/* Private functions ---------------------------------------------------------*/

static void clock_init()
{
	CLK_DeInit();
	CLK_ClockSwitchCmd(ENABLE);
	
	//Configure the HSI prescaler to DIV1
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	
	//Configure the CPU prescaler to DIV1
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
	
	//Configure the system clock to use HSI clock source
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	
	//Enable Peripheral Clock for ADC
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);    
	//Enable Peripheral Clock for TIMER4
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE); 
	//Enable Peripheral Clock for TIMER1
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
	
	//Disable Peripheral Clock for everything else
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, DISABLE);
}

static void peripherals_init()
{
	//Init gpios
	IO_Init();
	//Init adc1
	ADC1_DeInit();
	
	//Init timer4 for screen updates
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_PRESCALER_64, 249);
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	
	//Init timer1 for voltmeter and ampermeter values update
	TIM1_DeInit();
	TIM1_TimeBaseInit(256, TIM1_COUNTERMODE_UP, 6249, 0);
	TIM1_ClearFlag(TIM1_FLAG_UPDATE);
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	
	//Set lower interrupt priority for timer1
	ITC->ISPR3 &= ~0b11000000;
}

static void init_drivers()
{
	Display_IOs_t volmeter_ios =
	{
		{ io_DISPLAY_SEG_A, io_DISPLAY_SEG_B, io_DISPLAY_SEG_C, io_DISPLAY_SEG_D, io_DISPLAY_SEG_E, io_DISPLAY_SEG_F, io_DISPLAY_SEG_G, io_DISPLAY_SEG_DP },
		{ io_DISPLAY_VOLTMETER_DIGIT_1, io_DISPLAY_VOLTMETER_DIGIT_2, io_DISPLAY_VOLTMETER_DIGIT_3 }
	};
	
	Display_IOs_t ampermeter_ios =
	{
		{ io_DISPLAY_SEG_A, io_DISPLAY_SEG_B, io_DISPLAY_SEG_C, io_DISPLAY_SEG_D, io_DISPLAY_SEG_E, io_DISPLAY_SEG_F, io_DISPLAY_SEG_G, io_DISPLAY_SEG_DP },
		{ io_DISPLAY_AMPERMETER_DIGIT_1, io_DISPLAY_AMPERMETER_DIGIT_2, io_DISPLAY_AMPERMETER_DIGIT_3 }
	};
	
	//Init display for voltmeter and ampermeter
	display_init(&voltmeter_display, volmeter_ios);
	display_init(&ampermeter_display, ampermeter_ios);
	
	//Show 0 on voltmeter and ampermeter screen
	display_print_float(&voltmeter_display, 0);
	display_print_float(&ampermeter_display, 0);
}

void main(void)
{
	clock_init();
	peripherals_init();
	init_drivers();
	
	enableInterrupts();
	TIM4_Cmd(ENABLE);
	TIM1_Cmd(ENABLE);
	
	//Infinite loop
  while (1)
		_asm("nop");
}

// Voltmeter and ampermeter values update
@svlreg @far @interrupt void TIM1_UPD_OVF_TRG_IRQHandler(void)
{
	int i,j=0;
	float values[35];
	float values_double[35];
	
	//Print filtered voltmeter value
	for(i=0;i<35;i++)
	{
		for(j=0;j<35;j++)
			values[j] = voltmeter_get_voltage(ADC1_CHANNEL_4);
			
		insertion_sort(values, 35);
		values_double[i] = values[17];
	}
	insertion_sort(values_double, 35);
		
	display_print_float(&voltmeter_display, values_double[17]);
		
		
	//Print filtered ampermeter value
	for(i=0;i<35;i++)
	{
		for(j=0;j<35;j++)
			values[j] = ampermeter_get_current(ADC1_CHANNEL_3);
			
		insertion_sort(values, 35);
		values_double[i] = values[17];
	}
	insertion_sort(values_double, 35);
		
	display_print_float(&ampermeter_display, values_double[17]);

	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}


// Display digits update routine
@far @interrupt void TIM4_UPD_OVF_TRG_IRQHandler(void)
{
	static char current_display_id = 0;
	
	if(current_display_id == 0)
	{
		display_disable(&ampermeter_display);
		display_update(&voltmeter_display);
		current_display_id = 1;
	}
	else
	{
		display_disable(&voltmeter_display);
		display_update(&ampermeter_display);
		current_display_id = 0;
	}
	
	TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
