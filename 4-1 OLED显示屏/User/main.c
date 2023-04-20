#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"


int main(void)
{
	OLED_Init();
	
	OLED_ShowChar(1, 1, 'H');
	OLED_ShowString(1,3, "Hello STM32!");
	OLED_ShowNum(2, 1, 12345, 5);
	OLED_ShowSignedNum(2, 7, -520, 3); //有符号
	OLED_ShowHexNum(3, 1, 0xAA55, 4); 
	OLED_ShowBinNum(4, 1, 0xAA55, 16);
	
	//OLED_Clear();
	
	while (1)
	{
		
	}
}   