/**
 * if语句
 **/
#include <stdio.h>

int main(){
	
	// 定义局部变量
	int a = 30;

	if(a < 20){
		printf("a < 20 \n");
	}else if(a == 20){
		printf("a == 20\n");
	}else{
		printf("a > 20\n");
	}

	int b = intValue();
	printf("b 的值为: %d\n", b);

	return 0;
}

int intValue(){
	int a = 10;
	return (a % 2 == 0) ? 1 : 2;
}
