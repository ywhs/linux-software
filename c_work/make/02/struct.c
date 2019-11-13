#include <stdio.h>

typedef struct{
	int run;
	int walk;
	int eat;
} dog;

int main()
{

	// 初始化的一种方式
	dog dog1 = {
		.run = 1,
		.walk = 0,
		.eat = 0
	};

	showP(&dog1);

	return 0;
}

int showP(dog *dog1){

	printf ("dog of value: run = %d, walk = %d, eat = %d \n", dog1-> run, dog1->walk, dog1->eat);
	
	return 0;

}
