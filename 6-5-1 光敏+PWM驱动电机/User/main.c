#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LightSensor.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum;
int8_t Speed;

int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();
	LightSensor_Init();
	
	while (1)
	{
		//光线亮
		if (LightSensor_Get() == 0)
		{
				OLED_ShowString(1, 1, "Light:  ON");
				KeyNum = Key_GetNum();
			if (KeyNum == 1)
			{
				Speed += 20;
				if (Speed > 100)
				{
					Speed = -100;
				}
			}
			Motor_SetSpeed(Speed);
			OLED_ShowString(2, 1, "Speed:");
			OLED_ShowSignedNum(2, 7, Speed, 3);
			}
		else
		{
			OLED_ShowString(1, 1, "Light: OFF");
			Motor_SetSpeed(0);
			OLED_ShowString(2, 1, "Speed:   0");
		}
	}
}    