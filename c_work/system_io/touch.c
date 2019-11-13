#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	// 如果没有这个文件则创建这个文件，因为模式中我没有用打开的方式，而是用的是O_CREAT，创建文件
	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);
	
	// 判断文件是否被创建成功
	if(fd < 0){
		printf("create %s is fail!\n", argv[1]);
	}

	// 创建文件成功，但是根据touch命令的特性，并不需要打开文件，则关闭文件
	printf("create %s is sucess! fd is %d\n", argv[1], fd);

	//close(fd);

	return 0;

}
