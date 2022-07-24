#include "stm32f4xx.h"
#include "../../Drivers/inc/ADC.h"
#include "../../Drivers/inc/LCD.h"

unsigned short int Data;

int main(void)
{
	ADCConfigType conf = { Resolution12, ConversionMode_SINGLE, 6 };

	ADC_Init(&conf,1);

	while(1)
	{

#if USE_POLLING == 1
		unsigned char conv_state;
	    ADC_GetConversionState(&conv_state);
	    if (conv_state == 1){
		    ADC_ReadData(&Data);
		    Data = (Data*150*6)/(1.5*4095*4);
		    LCD_Init();
		    LCD_IntToStr(Data);
	    }
#endif

	}
}

void ISR(unsigned short int Data){
	ADC_ReadData(&Data);
	Data = (Data*150*6)/(1.5*4095*4);
	LCD_Init();
	LCD_IntToStr(Data);
}

#if USE_POLLING == 0

void ADC_IRQHandler(void)
{
	ISR(Data);
}

#endif
