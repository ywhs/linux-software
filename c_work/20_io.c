#include <stdio.h>

int main(){
	
	// 单个字符进行输入和输出
	printf("Enter a value:");
	//int c = getchar();
	
	printf("\nYou entered :");
	//putchar(c);
	printf("\n");

	// 字符串形式输入和输出，遇到回车则结束输入
	char c[100];
    printf("Enter a value:");
	fgets( c,100,stdin );
	printf("\nyou entered:");
	fputs( c,stdout );
	
	// 使用scanf() 和 printf() 进行输入和输出
	har 
	
	return 0;

}
