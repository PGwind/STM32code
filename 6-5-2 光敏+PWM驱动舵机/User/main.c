#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LightSensor.h"
#include "Servo.h"
#include "Key.h"

uint8_t KeyNum;
float Angle;

int main(void)
{
	OLED_Init();
	Key_Init();
	Servo_Init();
	LightSensor_Init();
	OLED_ShowString(2, 1, "Angle:");
	while (1)
	{
		//光线亮
		if (LightSensor_Get() == 0)
		{
			OLED_ShowString(1, 1, "Light:  ON");
			KeyNum = Key_GetNum();
			if (KeyNum == 1)
			{	
				Angle += 30;
				if (Angle > 180)
				{
					Angle = 0;
				}
				Servo_SetAngle(Angle);
				OLED_ShowNum(2, 7, Angle, 3);
			}
		}
		else
		{
			OLED_ShowString(1, 1, "Light: OFF");
			Servo_SetAngle(0);
			OLED_ShowString(2, 7, "000");
		}
	}   
}	