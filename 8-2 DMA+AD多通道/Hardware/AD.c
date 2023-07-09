#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];

//ADC触发DMA
void AD_Init(void)
{
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	//ADC初始化
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //无内部触发：软件触发
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 4; //通道数 1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //外设站点
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value; //存储站点
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设->存储
	DMA_InitStructure.DMA_BufferSize = 4; //传输次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //循环模式
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //硬件触发，触发源：ADC1
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1); //复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET); //返回状态
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

}

//void AD_GetValue(void)
//{
//	DMA_Cmd(DMA1_Channel1, DISABLE); //失能
//	DMA_SetCurrDataCounter(DMA1_Channel1, 4); //设置DMA当前数据传输计数器的值
//	DMA_Cmd(DMA1_Channel1, ENABLE); //使能
//	
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //启动  单次
//	
//	while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); //等待转运完成，标志位置1
//	DMA_ClearFlag(DMA1_FLAG_TC1);
//}
