#include <stdio.h>

int main(){
	
	// 定义数组指针,在生命指针数组时，必须指定容量多少才可以
	int i, *ptr[3];
	int var[] = {1,2,3};
	// 循环数组把数据地址给指针数组
	for(i = 0; i < 3; i++){
		ptr[i] = &var[i];
	}

	// 循环输出
	for(i = 0; i < 3; i++){
		printf("Value of var[%d] = %d \n", i, *ptr[i]);
	}

	return 0;
}
