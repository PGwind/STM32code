#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "stdarg.h"

void Serial_Init(void)
{
	// 开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化引脚
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用功能推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 初始化USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600; // 波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	// 启用
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte) // 发送16进制或字节
{
	USART_SendData(USART1, Byte); //DR(寄存器) -> TDR(发送数据寄存器) -> 发送移位寄存器 
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 如果是RESET则一直循环，直到为SET
}

void Serial_SendArray(uint8_t *Array, uint16_t Length) // 发送数组
{ 
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String) // 字符串(自带结束标志位)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y) // 取数最低位算法
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length) // 发送字符数字
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0'); 
	}
}

int fputc(int ch, FILE *f) // printf()移植
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...) // 通过可变参数  封装
{
	char String[100];
	va_list arg; // 参数列表变量
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg); // 释放参数列表
	Serial_SendString(String);
}