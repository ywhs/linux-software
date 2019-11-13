#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{

	int x, y;
	int result1, result2;

	x = 10;
	y = 20;

	result1 = function1(x, y);
	result2 = function2(x, y);

	printf("function1: %u \n", result1);
	printf("function2: %u \n", result2);

	return 0;


}
