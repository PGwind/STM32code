#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"
#include "MPU6050.h"

int main(void)
{
	OLED_Init();
	MPU6050_Init();
	


	while (1)
	{
		
	}
}   