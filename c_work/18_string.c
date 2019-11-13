#include <stdio.h>
#include <string.h>

int main(){
	
	char greeting[6] = {'h', 'e', 'l', 'l', 'o', '\0'};

	printf("value of greeting: %s \n", greeting);

	printf("=========================================运用字符串函数 \n");

	char str1[12] = "hello";
	char str2[12] = "world";
	char str3[12];
	int len;

	// 把s1的值赋给s3
	strcpy(str3, str1);
	printf("赋值函数strcpy(str3, str1) : %s \n", str3);

	// 连接str1 和 str2,str2到str1的末尾
	strcat(str1, str2);
	printf("连接两个字符串函数strcat(str1. str2) : %s \n", str1);

	// 连接后，str1的长度
	len = strlen(str1);
	printf("计算字符串长度函数strlen(str1) : %d \n", len);


	char str4 = 'e';
	char *p;

	p = strchr(str1, str4);
	int pos = p-str1;

	printf("返回str4在str1的第一次出现位置函数strchr(str1, str4)：%d \n", pos);


	return 0;

}
