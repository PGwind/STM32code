#include <stdio.h>
#include <time.h>

time_t time_cnt;
struct tm time_date;
char *time_str;

int main()
{ 
	// time_cnt = time(NULL);
	time(&time_cnt); // 获取时间戳 
	
	printf("%d\n", time_cnt);
	
	// time_date = *gmtime(&time_cnt); //标志时间 
	time_date = *localtime(&time_cnt); // 东八区北京时间 
	printf("%d年\n", time_date.tm_year + 1900);
	printf("%d月\n", time_date.tm_mon + 1);
	printf("%d日\n", time_date.tm_mday);
	printf("%d时\n", time_date.tm_hour);
	printf("%d分\n", time_date.tm_min);
	printf("%d秒\n", time_date.tm_sec);
	printf("星期%d\n", time_date.tm_wday);
	
	time_cnt = mktime(&time_date); // 日期 -> 时间戳 
	printf("%d\n", time_cnt);
	
	time_str = ctime(&time_cnt); // 时间戳 -> 日期
	printf(time_str); 
	
	time_str = asctime(&time_date); // asctime函数需要你自己提供struct tm结构体，而不是直接传递时间戳
	printf(time_str);
	
	char t[50];
	strftime(t, 50, "%H-%M-%S", &time_date); // 将时间戳转换为格式化的时间字符串
	printf(t);
	
	return 0;
}
 
