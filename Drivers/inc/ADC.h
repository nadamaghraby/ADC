#ifndef INC_ADC_H_
#define INC_ADC_H_

#define USE_POLLING 1

typedef enum {
	Resolution12,
	Resolution10,
	Resolution08,
	Resolution06,
} Resolution;

typedef  enum {
	ConversionMode_SINGLE,
	ConversionMode_CONTINUES,
} ConversionMode;

typedef struct {
	Resolution res;
	ConversionMode conv;
	char reference_voltage;
} ADCConfigType;

void ADC_Init(ADCConfigType* ConfigParamPtr, int ChannelNum);

void ADC_StartConv(unsigned char ChannelNum);

#if USE_POLLING == 1
void ADC_GetConversionState(unsigned char* ConversionStatePtr);
#endif

unsigned char ADC_ReadData(unsigned short int* DataPtr);

#endif /* INC_ADC_H_ */
