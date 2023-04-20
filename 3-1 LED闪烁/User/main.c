#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main(void)
{
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
		//点亮
 		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		//GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
	  GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
		Delay_ms(500); 
		
		//熄灭
		//GPIO_SetBits(GPIOA, GPIO_Pin_0);
		//GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		Delay_ms(500);
	}
}
