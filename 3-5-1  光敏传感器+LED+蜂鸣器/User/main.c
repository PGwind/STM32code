#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LightSensor.h"
#include "LED.h"
#include "OLED.h"
#include "Buzzer.h"

int main(void)
{
	LED_Init();
	OLED_Init();
	Buzzer_Init();
	LightSensor_Init();
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//GPIO挂载在APB2总线上
	//GPIO:General Purpose Input Output(通用输入输出口 )
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	while (1)
	{
		/* 由于上一次的光线状态被保存在静态变量 lastState 中，
		因此当下一次循环时，可以将当前的光线状态与上一次的光线状态进行比较，
		以便检测光线状态是否发生变化。这样可以避免蜂鸣器一直响或只响一次的问题 */
	
		static uint8_t lastState = 0; //上一次光线状态
		uint8_t nowState = LightSensor_Get(); //现在的光线状态

			if (nowState != lastState)
			{
					lastState = nowState;

					if (nowState == 1) // 光线暗
					{
							GPIO_ResetBits(GPIOA, GPIO_Pin_1);
							GPIO_SetBits(GPIOA, GPIO_Pin_2);
							OLED_ShowString(1,1, "LED: Red   ON");
							Buzzer_ON();
							Delay_ms(50);
							Buzzer_OFF();
					}
					else // 光线亮
					{
							GPIO_ResetBits(GPIOA, GPIO_Pin_2);
							GPIO_SetBits(GPIOA, GPIO_Pin_1);
							OLED_ShowString(1,1, "LED: White ON");
							Buzzer_ON();
							Delay_ms(50);
							Buzzer_OFF();
					}
			}
	}
	
	
	/*错误代码
	while (1)
	{
		//光线暗
		if (LightSensor_Get() == 1)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			OLED_ShowString(1,1, "LED: Red   ON");
			Buzzer_ON();
			Delay_ms(50);
			Buzzer_OFF();
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			OLED_ShowString(1,1, "LED: White ON");
			Buzzer_ON();
			Delay_ms(50);
			Buzzer_OFF();
		}
	}  
	*/ 

	
}
