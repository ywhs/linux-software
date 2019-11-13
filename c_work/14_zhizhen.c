#include <stdio.h>

int main(){

	int *p = NULL;

	printf("Address of p variable : %p \n", p);

	if(p){
		printf("闯关成功！\n");
	}else{
		printf("空指针异常！\n");
	}

	printf("-----------------------------------------------------\n");

	test();
	
	return 0;
}

test(){

	int  *ptr = NULL;
	if(ptr){
		printf("ptr 的地址是 %p\n", ptr);
	} 
	 
}
