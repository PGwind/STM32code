#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // TIM3的通道1引脚为PA6
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3); //选择内部时钟
	
	//初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//PSC预分频器用于分频输入时钟，ARR计数器用于记录计数值，而自动重装器用于设置计数器的上限值
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1; //ARR 65536，防止计数器溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;//PSC预分频器  72M/7200 = 10k  计数标准频率
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器（高级定时器）
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF; //输入捕获滤波器
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //极性，选择上升沿/下降沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //分频器, 不分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); //触发源选择
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); //从模式
	
	TIM_Cmd(TIM3, ENABLE); //启动定时器
}

// 最新一个周期的频率值
uint32_t IC_GetFreq(void) 
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}
