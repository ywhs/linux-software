#include <stdio.h>

// 定义全局变量
const int MAX = 6;

int main(){
	
	// 定义数组
	int var[] = {1,2,3,4,5,100};
	// 定义指针变量
	int i, *ptr;
	// 把var数组的地址赋值给指针变量,如果是整个数组变量的地址，则无需使用&
	ptr = var;


	// 循环数组, 在c89版本中，for循环中的初始值，必须在外侧定义，c99版本可以在for循环体中定义
	for(i = 0; i < MAX; i++ ){
		printf("存储地址: var[%d] = %x \n", i, ptr);
		printf("存储值 : var[%d] = %d \n ", i, *ptr);
		// 移动到下一个位置
		ptr++;
	}
	
	printf("--------------------------------------------------------------------\n");

	charTest();

	return 0;

}

/**
 * 测试char类型的指针变量递增后是否递增 1
 **/
charTest(){
	
	char *p;
	char var[] = {'1','2','3','4','5'};
	int i;
	p = var;

	for(i = 0; i < MAX; i++){
		printf("存储地址: var[%d] = %x \n", i, p);
		printf("存储值 : var[%d] = %d \n", i, *p);
		p++;
	}
}
