#include <stdio.h>
#include <stdlib.h>

int main(){
	int arr[] = {1,2,3,4,5};

	printf("数组总长度（字节）: %d \n", sizeof(arr));
	printf("数组中的某一个字节长度：%d \n", sizeof(arr[0]));

	printf("数组总共有多少个元素: %d \n", (sizeof(arr) / sizeof(arr[0])));
	return 0;
}
