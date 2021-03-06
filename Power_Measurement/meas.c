#include "stm32f10x.h"                  // Device header
#include <stdbool.h>
#include "adc.h"
#include "meas.h"

#define offsetVoltage 1500
#define sensitivity		185

#define CURRENT_CHANNEL 		ADC_CHANNEL2
#define VOLTAGE_CHANNEL			ADC_CHANNEL1

void ADC_Measurement_Init(void)
{
	ADC_Single_Conv_Init(ADC1, VOLTAGE_CHANNEL, ADC_CHANNEL1_MAX_SAMPLE_TIME);
	ADC_Single_Conv_Init(ADC2, CURRENT_CHANNEL, ADC_CHANNEL2_MAX_SAMPLE_TIME);
	
}

static void store_ADC_Voltage(uint16_t* values)
{
    for(int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
      values[i] = ADC_Read(ADC1);
    }
}

static void store_ADC_Current(uint16_t* values)
{
    for(int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
      values[i] = ADC_Read(ADC2);
    }
}

static uint16_t getMaximumValue(uint16_t* values)
{
    uint16_t maxValue = values[0];
    for(int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        uint16_t currentVal = values[i];
        if(currentVal > maxValue)
        {
            maxValue = currentVal;
        }
    }
    return maxValue;
}

double getVoltage(uint16_t* adcVal)
{
	store_ADC_Voltage(adcVal);
	uint16_t maximum = getMaximumValue(adcVal);
  double conv = (maximum * 3.3)/4095.0;
  double rms = conv/1.414;
  double voltage = (((480+1)) * rms) / 4.7;
  return voltage;
}

double getCurrent(uint16_t* adcVal)
{
	store_ADC_Current(adcVal);
	uint16_t maximum = getMaximumValue(adcVal);
	double conv = (maximum * 3000.0) / 4095.0;		//gives us value in mV
	double current = ((conv - offsetVoltage) / sensitivity);
	double ecurrent = current / 1.414;
	return ecurrent - 3.0;
	
}


