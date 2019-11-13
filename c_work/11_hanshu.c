#include <stdio.h>

int a = 10;

int main(){
	
	int a = 20;

	int b = 10;

	int c = sum(a, b);
	printf ("value of a in main() = %d\n",  a);
	printf("value of c in sum() = %d\n", c);

	return 0;

}

int sum(int a, int b){
	printf("value of a in sum() = %d\n", a);
	printf("value of b in sum() = %d\n", b);
	return a + b;
}
