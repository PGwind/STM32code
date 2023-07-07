#include "stm32f10x.h"                  // Device header

//extern uint16_t Num;

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2); //内部时钟
	
	//时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//PSC预分频器用于分频输入时钟，ARR计数器用于记录计数值，而自动重装器用于设置计数器的上限值
	TIM_TimeBaseInitStructure.TIM_Period = 10000-1; //ARR自动重装器 10000/10k = 1s
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200-1;//PSC预分频器  72M/7200 = 10k
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器（高级定时器）
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	// 清除定时器 TIM2 的更新标志位,以便在下一次更新事件发生时重新设置标志位
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//使能中断:使能定时器 TIM2 的更新中断功能，以便在定时器触发更新事件时触发相应的中断处理程序
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	//NVIC 配置中断优先级分组为 Group 2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

//中断函数
/*void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num ++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}*/