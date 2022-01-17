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
	
}room_t;

room_t room1 = {0};

int32_t count1;
int32_t count2;

uint16_t adcVoltage[NUMBER_OF_SAMPLES] = {0};
uint16_t adcCurrent[NUMBER_OF_SAMPLES] = {0};
static double difference;
static double old;

int main(void)
{
	System_Init();
	voltage_CurrentMeasurement_Init();
	voltage_Init();
	current_Init();
	
	while(1)
	{
		room1.voltage = getVoltage(adcVoltage);
		room1.current = getCurrent(adcCurrent);
		
		if(get_Flag_Status())
		{
			get_Voltage_Timer_Value(&count1);
			get_Current_Timer_Value(&count2);
			
			difference = abs(count1 - count2);
			
			if(difference < 20)
			{
				old = difference;
			}
			
			room1.phase_diff = ((old/20.0) * 2 * 3.14) - 3.14;
				
			room1.pfactor = cos(room1.phase_diff );
			room1.active_pwr = fabs((room1.current * room1.voltage * room1.pfactor));
			room1.apparent_pwr = (room1.current * room1.voltage);
			room1.reactive_pwr = fabs((room1.current * room1.voltage * sin(room1.phase_diff)));
			
			reset_Flag_Status();
			System_TimerDelayMs(5);
			
		}
	}
}
