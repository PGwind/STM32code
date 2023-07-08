#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	//时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//GPIO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	//ADC初始化
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //无内部触发：软件触发
	//ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //非扫描模式
	ADC_InitStructure.ADC_NbrOfChannel = 1; //通道数 1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//上电
	ADC_Cmd(ADC1, ENABLE);
	
	//校准
	ADC_ResetCalibration(ADC1); //复位校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET); //返回状态
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //启动  连续
}

uint16_t AD_GetValue(void)
{
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE); //启动  单次
	//while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //转换未完成  等待
	return ADC_GetConversionValue(ADC1); //读取DR寄存器后自动清除EOC标志位   读取
}
