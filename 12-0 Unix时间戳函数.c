#include <stdio.h>
#include <time.h>

time_t time_cnt;
struct tm time_data;
char *time_str;

int main()
{ 
	// time_cnt = time(NULL);
	time(&time_cnt); // ��ȡʱ��� 
	
	printf("%d\n", time_cnt);
	
	// time_data = *gmtime(&time_cnt); //��־ʱ�� 
	time_data = *localtime(&time_cnt); // ����������ʱ�� 
	printf("%d��\n", time_data.tm_year + 1900);
	printf("%d��\n", time_data.tm_mon + 1);
	printf("%d��\n", time_data.tm_mday);
	printf("%dʱ\n", time_data.tm_hour);
	printf("%d��\n", time_data.tm_min);
	printf("%d��\n", time_data.tm_sec);
	printf("����%d\n", time_data.tm_wday);
	
	time_cnt = mktime(&time_data); // ���� -> ʱ��� 
	printf("%d\n", time_cnt);
	
	time_str = ctime(&time_cnt); // ʱ��� -> ����
	printf(time_str); 
	
	time_str = asctime(&time_data); // asctime������Ҫ���Լ��ṩstruct tm�ṹ�壬������ֱ�Ӵ���ʱ���
	printf(time_str);
	
	char t[50];
	strftime(t, 50, "%H-%M-%S", &time_data); // ��ʱ���ת��Ϊ��ʽ����ʱ���ַ���
	printf(t);
	
	return 0;
}
 
