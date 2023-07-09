#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//外设和存储只是命名
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA; //外设站点
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB; //存储站点
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设->存储
	DMA_InitStructure.DMA_BufferSize = Size; //传输次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ; //不自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; //软件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
}

void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE); //失能
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size); //设置DMA当前数据传输计数器的值
	DMA_Cmd(DMA1_Channel1, ENABLE); //使能
	
	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); //等待转运完成，标志位置1
	DMA_ClearFlag(DMA1_FLAG_TC1);
}