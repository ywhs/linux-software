/**
 * 定义常量
 **/
#include <stdio.h>

#define LENGTH 10
#define WIDTH  5
#define NEWLINE '\n'

 int main(){
 	
	int area;
	area = LENGTH * WIDTH;
	printf("value of area : %d", area);
	printf("%c", NEWLINE);
	int result = areaValue();
	printf("value1 of area : %d", result);
	printf("%c", NEWLINE);

	return 0;

 }

 int areaValue(){
 	
	const LENGTH1 = 20;
	const WDITH = 10;

	int area1 = LENGTH1 * WDITH;

	return area1;

 }
