#include "stm32f10x.h"                  // Device header

void MySPI_W_SS(uint8_t BitValue) // CS
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue) // SCK
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue) // MOSI
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}

uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);
}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

/* 
uint8_t MySPI_SwapByte(uint8_t ByteSend) // 移位
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i++)
	{
		MySPI_W_MOSI(ByteSend & 0x80); 
		ByteSend <<= 1; // ByteSend最高位移出到MOSI
		MySPI_W_SCK(1); // SCK上升沿
		if (MySPI_R_MISO() == 1) {ByteSend |= 0x01;} // 主机接收从机次高位
		MySPI_W_SCK(0); // SCK下降沿
	}
	
	return ByteReceive;
}
*/

// 模式0
uint8_t MySPI_SwapByte(uint8_t ByteSend) // 利用掩码依次提取数据每一位
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i)); 
		MySPI_W_SCK(1); // SCK上升沿
		if (MySPI_R_MISO() == 1) {ByteReceive |= (0x80 >> i);} // 主机接收从机次高位
		MySPI_W_SCK(0); // SCK下降沿
	}
	
	return ByteReceive;
}

/*
// 模式1
{
	uint8_t i, ByteReceive = 0x00;
	
	for (i = 0; i < 8; i++)
	{
		MySPI_W_SCK(1);
		MySPI_W_MOSI(ByteSend & (0x80 >> i)); 
		MySPI_W_SCK(0);
		if (MySPI_R_MISO() == 1) {ByteReceive |= (0x80 >> i);} 
	}
	
	return ByteReceive;
}

// 模式3：修改模式1 SCK极性相反即可
// 模式2：修改模式0 SCK极性相反即可

*/


