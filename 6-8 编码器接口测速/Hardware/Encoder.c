#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // TIM3的通道1引脚为PA6
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;   //PA6和7· 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//PSC预分频器用于分频输入时钟，ARR计数器用于记录计数值，而自动重装器用于设置计数器的上限值
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1; //ARR 65536，防止计数器溢出
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;//PSC    不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//重复计数器（高级定时器）
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure; //定义结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure); //给结构体赋初始值
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF; //输入捕获滤波器
	TIM_ICInit(TIM3, &TIM_ICInitStructure); //写入寄存器
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF; //输入捕获滤波器
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0); //CNT清零
	return Temp;
}