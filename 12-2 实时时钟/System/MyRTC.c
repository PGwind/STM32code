#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2023, 7, 26, 14, 46, 35};
void MyRTC_SetTime();

void MyRTC_Init(void)
{
	// RTC初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // 开启PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE); // 开启BKP时钟
	
	PWR_BackupAccessCmd(ENABLE); // 设置PWR_CR，使能对BKP和RTC的访问
	
	// 判断是否完全断电
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) 
	{
		RCC_LSEConfig(RCC_LSE_ON); // 启动LSE
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
	
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // 选择RTC时钟源：LSE(外部低速时钟)
		RCC_RTCCLKCmd(ENABLE);
	
		RTC_WaitForSynchro(); // 等待同步
		RTC_WaitForLastTask(); // 等待上一次操作完成
		
		RTC_SetPrescaler(32768 - 1); // 设置分频系数
		RTC_WaitForLastTask();
	
		// RTC_SetCounter(1672588795); // 设置初始时间
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
	
}

void MyRTC_SetTime()
{
	time_t time_cnt;
	struct tm time_date;
	
	time_date.tm_year = MyRTC_Time[0] - 1900;
	time_date.tm_mon = MyRTC_Time[1] -1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date); // 日期 -> 时间戳
	
	RTC_SetCounter(time_cnt); // 时间戳写入RTC的CNT中
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter(); // 读取
	
	time_date = *localtime(&time_cnt); // 时间戳 -> 日期
	
	MyRTC_Time[0] = time_date.tm_year + 1900;
	MyRTC_Time[1] = time_date.tm_mon + 1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}