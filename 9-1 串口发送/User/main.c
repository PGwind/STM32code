#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	
	Serial_Init();
	
	// 16进制
	Serial_SendByte(0x41);
	Serial_SendByte(0x20); // ASCII 空格
	Serial_SendByte(0x42); // TX引脚产生一个0x41对应的波形
	
	// 字符
	Serial_SendByte(' ');
	Serial_SendByte('A');
	Serial_SendByte('B');
	Serial_SendByte(' ');
	
	// 数组
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45, 0x20};
	Serial_SendArray(MyArray, 5);
	
	// 字符串(字符数组)
	Serial_SendString("\r\nHell0 World!\r\n"); // \r\n 换行
	
	// 字符数字
	Serial_SendNumber(12345, 5);

	// 调用printf()
	printf("\r\nNum = %d\r\n", 666);
	
	// 未封装
	char String[100];
	sprintf(String, "Num = %d\r\n", 777);
	Serial_SendString(String);
	
	// 封装
	Serial_Printf("Num = %d\r\n", 888);
	
	// UTF-8
	Serial_Printf("你好，世界");
	
	while (1)
	{
		
	}
}   