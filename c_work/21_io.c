#include <stdio.h>

int main(){

	char str[20];
	int *i;

	printf("Enter a value :");
	scanf("%s %d", str, &i);

	printf("\nYou entered : %s %d", str, i);
	printf("\n");

	return 0;

}
