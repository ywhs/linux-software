/**
 * 数据类型
 * */
#include <stdio.h>
#include <limits.h>
#include <float.h>


int main(){

	printf("int 存储大小为：%lu \n", sizeof(int) );
	printf("float 存储大小为： %E \n", sizeof(float) );
	printf("float 最大值为：%E \n", FLT_MAX );
	printf("float 最小值为： %E \n", FLT_MIN );
	printf("精度值：%d \n", FLT_DIG );
	return 0;
}
