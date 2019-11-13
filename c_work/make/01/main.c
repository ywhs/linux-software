#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	
	// 定义两个变量
	int x,y;

	// 从字符串读取格式化输入 %u代表着无符号 int
	sscanf(argv[1], "%u", &x);
	sscanf(argv[2], "%u", &y);

	// 调用其他文件中的函数
	printf("function1 两数之和为: %u \n", function1(x,y));
	printf("function2 两数的乘积为: %u \n", function2(x,y));

	return 0;


}
