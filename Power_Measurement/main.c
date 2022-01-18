#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "system.h"
#include "lm358.h"
#include "meas.h"
#include <math.h>


/*
CURRENT SENSOR: PA2
VOLTAGE DIVIDER: PA1
Current Op-amp: PA4
Voltage Op-amp: PA0
*/

typedef struct
{
	double voltage;
	double current;
	double phase_diff;
	double pfactor;
	double apparent_pwr;
	double active_pwr;
	double reactive_pwr;
	
}meter_t;

meter_t meter = {0};

int32_t count1;
int32_t count2;

uint16_t adcVoltage[NUMBER_OF_SAMPLES] = {0};
uint16_t adcCurrent[NUMBER_OF_SAMPLES] = {0};
static double difference;
static double old;

int main(void)
{
	System_Init();
	ADC_Measurement_Init();
	LM358_Voltage_Init();
	LM358_Current_Init();
	
	while(1)
	{
		meter.voltage = getVoltage(adcVoltage);
		meter.current = getCurrent(adcCurrent);
		
		if(get_Flag_Status())
		{
			get_Voltage_Timer_Value(&count1);
			get_Current_Timer_Value(&count2);
			
			difference = abs(count1 - count2);
			
			if(difference < 20)
			{
				old = difference;
			}
			
			/*
			* Phase difference is ((time difference between two waves) / period) * 2 * pi
			*/
			meter.phase_diff = ((old/20.0) * 2 * 3.14) - 3.14; // Subtraction is done to make result positive.
			meter.pfactor = cos(meter.phase_diff );
			meter.active_pwr = fabs((meter.current * meter.voltage * meter.pfactor));
			meter.apparent_pwr = (meter.current * meter.voltage);
			meter.reactive_pwr = fabs((meter.current * meter.voltage * sin(meter.phase_diff)));
			
			reset_Flag_Status();
			System_TimerDelayMs(5);
			
		}
	}
}
