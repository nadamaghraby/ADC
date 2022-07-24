#include "stm32f4xx.h"
#include "../inc/ADC.h"
#include "../inc/gpio.h"

void ADC_Init(ADCConfigType* ConfigParamPtr,int ChannelNum) {

	// Enable clock to GPIOA
	RCC->AHB1ENR |=(1u<<0);

	// pin 1 to analog mode
	GPIOA->MODER |= (1u<<(ChannelNum+1));
	GPIOA->MODER |= (1u<<(ChannelNum+2));

	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1;

	// Enable clock access to ADC1
	RCC->APB2ENR |=(1u<<8);

	ADC1->CR2 &= ~ADC_CR2_ALIGN;

	//set the number of channels
	ADC1->SQR1 &= ~(1u<<20);

	// Enable the ADC
	ADC1->CR2 |= (1u<<0);

	// Resolution
	if (ConfigParamPtr->res == Resolution12 ){
		ADC1->CR1 &= ~ADC_CR1_RES;
	}else if (ConfigParamPtr->res == Resolution10){
		ADC1->CR1 &= ~ADC_CR1_RES;
		ADC1->CR1 |= ADC_CR1_RES;
	}else if (ConfigParamPtr->res == Resolution08){
		ADC1->CR1 |= ADC_CR1_RES;
		ADC1->CR1 &= ~ADC_CR1_RES;
	}else if (ConfigParamPtr->res == Resolution06){
		ADC1->CR1 |= ADC_CR1_RES;
		ADC1->CR1 |= ADC_CR1_RES;
	}

//	 conversion
	if (ConfigParamPtr->conv == ConversionMode_SINGLE){
		ADC1->CR2 &= ~(1u<<1);
	}else if(ConfigParamPtr->conv == ConversionMode_CONTINUES){
	    ADC1->CR2 |= (1u<<1);
	}

	// enable interrupt for regular channels
#if USE_POLLING == 0
	ADC1->CR1 |= ((1u<<5));
	NVIC->ISER[0]|=(1u<<18);
#endif

	ADC_StartConv(ChannelNum);
}

void ADC_StartConv(unsigned char ChannelNum){
	if (ChannelNum == 0){
		// set the order of channels using SQR register
		ADC1->SQR3 &= ~(1u<<0);
		ADC1->SQR3 &= ~(1u<<1);
		ADC1->SQR3 &= ~(1u<<2);
		ADC1->SQR3 &= ~(1u<<3);
		ADC1->SQR3 &= ~(1u<<4);
	}else if (ChannelNum == 1){
		ADC1->SQR3 |= (1u<<0);
		ADC1->SQR3 &= ~(1u<<1);
		ADC1->SQR3 &= ~(1u<<2);
		ADC1->SQR3 &= ~(1u<<3);
		ADC1->SQR3 &= ~(1u<<4);
	}
	// Start conversion of regular channels
	ADC1->CR2 |= (1u<<30);
}

#if USE_POLLING == 1
void ADC_GetConversionState(unsigned char* ConversionStatePtr){
	if ((ADC1->SR & ADC_SR_EOC) == (ADC_SR_EOC)){
		*ConversionStatePtr = 1;
	}else{
		*ConversionStatePtr = 0;
	}
}
#endif

unsigned char ADC_ReadData(unsigned short int* DataPtr){
	* DataPtr = ADC1->DR;
	ADC1->CR2 |= (1u<<30);
	return 0;
}
