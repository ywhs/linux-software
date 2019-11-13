#include <stdio.h>
#include <time.h>

#define TIME 1000000000
// 全局变量
int m,n = TIME;

int main(){
	// 定义事件类型变量
	time_t start, stop;
	// 定义寄存器变量
	register int a,b = TIME;
	// 定义一般变量
	int x, y = TIME;

	// 计算寄存器变量用时多少
	time(&start);
	for(a = 0; a < b; a++);
	time(&stop);
	printf("寄存器变量用时：%ld 秒\n", stop - start);


	// 一般变量用时多少
	time(&start);
	for(x = 0; x < y; x++);
	time(&stop);
	printf("一般变量用时：%ld 秒\n", stop - start);


	// 全局变量用时
	time(&start);
	for(m = 0;  m < n; m++);
	time(&stop);
	printf("全局变量用时： %ld 秒\n", stop - start);


	return 0;

}
