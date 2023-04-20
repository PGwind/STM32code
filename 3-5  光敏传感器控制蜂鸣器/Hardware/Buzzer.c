#include "stm32f10x.h"                  // Device header

void Buzzer_Init(void)
{
	//在STM32中，每个外设都需要启用时钟才能工作，启用时钟的函数是 RCC_APB2PeriphClockCmd
	//该函数的第一个参数为外设的时钟线，第二个参数为使能或禁用该时钟线
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIO_SetBits函数的第一个参数是要设置的GPIO端口，第二个参数是要设置的GPIO引脚位。
	//同时将GPIOA的1号引脚和2号引脚设置为高电平
	GPIO_SetBits(GPIOB, GPIO_Pin_12); //初始化led默认熄灭 
}

void Buzzer_ON(void)
{
	//指定 GPIO 端口的引脚电平设置为低电平
	//两个参数：端口和引脚编号
	GPIO_ResetBits(GPIOB, GPIO_Pin_12); //引脚拉低(点亮)
}

void Buzzer_OFF(void)
{
	//指定 GPIO 端口的引脚电平设置为高电平
	GPIO_SetBits(GPIOB, GPIO_Pin_12); //引脚拉高(熄灭)
}

void Buzzer_Turn(void)
{
	//如果0低电平，即亮
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12) == 0)
	{
		//高电平熄灭
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
	else 
	{
		//低电平亮
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	}
}
