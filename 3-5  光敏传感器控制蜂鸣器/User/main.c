#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LightSensor.h"
#include "Buzzer.h"

uint8_t KeyNum;

int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	
	while (1)
	{
		//光线暗
		if (LightSensor_Get() == 1)
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
	}
}   