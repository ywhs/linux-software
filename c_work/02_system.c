/**
 * 使用system函数调用liunx命令
 **/
#include <stdio.h>
#include <stdlib.h>

int main(){

	printf("使用system命令运行liunx的命令！\n");
	system("./waibu");
	printf("运行完毕之后ls命令！\n");
	return 0;	
}
