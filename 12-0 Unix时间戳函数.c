#include <stdio.h>
#include <time.h>

time_t time_cnt;
struct tm time_date;
char *time_str;

int main()
{ 
	// time_cnt = time(NULL);
	time(&time_cnt); // ��ȡʱ��� 
	
	printf("%d\n", time_cnt);
	
	// time_date = *gmtime(&time_cnt); //��־ʱ�� 
	time_date = *localtime(&time_cnt); // ����������ʱ�� 
	printf("%d��\n", time_date.tm_year + 1900);
	printf("%d��\n", time_date.tm_mon + 1);
	printf("%d��\n", time_date.tm_mday);
	printf("%dʱ\n", time_date.tm_hour);
	printf("%d��\n", time_date.tm_min);
	printf("%d��\n", time_date.tm_sec);
	printf("����%d\n", time_date.tm_wday);
	
	time_cnt = mktime(&time_date); // ���� -> ʱ��� 
	printf("%d\n", time_cnt);
	
	time_str = ctime(&time_cnt); // ʱ��� -> ����
	printf(time_str); 
	
	time_str = asctime(&time_date); // asctime������Ҫ���Լ��ṩstruct tm�ṹ�壬������ֱ�Ӵ���ʱ���
	printf(time_str);
	
	char t[50];
	strftime(t, 50, "%H-%M-%S", &time_date); // ��ʱ���ת��Ϊ��ʽ����ʱ���ַ���
	printf(t);
	
	return 0;
}
 
