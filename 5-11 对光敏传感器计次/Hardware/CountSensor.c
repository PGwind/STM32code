#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void LightSensor_Init(void)
{
	//外部中断信号->GPIO->AFIO->EXTI->NVIC
	
	
	//EXIT 和 NVIC 时钟一直为打开着
	//RCC控制外设（EXIT虽然是外设，却开着）
	//配置时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//AFIO:可以用来重新映射每个可用GPIO端口的功能，这使得它们可以连接到不同的I/O端口或其它的外设上
	//GPIO_PortSourceGPIOB 是 GPIO B端口的引脚配置寄存器地址
	//GPIO_PinSource14参数表示外部中断线4所连接的引脚 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	//EXTI用于处理来自外部硬件的中断请求。
	//在外部中断发生时，通过EXTI控制器的相关寄存器进行处理，可以触发中断处理程序执行
	EXTI_InitTypeDef EXTI_InitStructure;
	//选择外部中断线
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	//使能或禁止指定外部中断线，这里使能 EXTI_Line14
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//选择外部中断模式，这里选择的是 EXTI_Mode_Interrupt，表示中断模式
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//选择外部中断触发方式，这里选择的是 EXTI_Trigger_Falling，表示下降沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	//调用 EXTI_Init 函数，将 EXTI_InitStructure 结构体的配置应用到外部中断 EXTI_Line14 上。
	EXTI_Init(&EXTI_InitStructure);
	
	//NVIC:Nested Vectored Interrupt Controller:用于管理和分配中断请求的优先级和响应
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //此方式整个芯片只能用一次
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

//中断函数无需申明，自动执行
void EXTI15_10_IRQHandler(void) ////中断函数名
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
		{
			CountSensor_Count ++;
		}
		////清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}



