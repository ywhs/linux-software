#include <stdio.h>
#include <libgen.h>

int main(int argc, char **argv)
{

	printf("value: %s \n", basename(argv[0]));

	return 0;

}
