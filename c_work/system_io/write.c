#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

	// 定义写入文件后的接收值
	int wr_ret;
	// 如果没有这个文件则创建这个文件，因为模式中我没有用打开的方式，而是用的是O_CREAT，创建文件
	int fd = open(argv[1], O_CREAT | O_RDWR, 0777);

	// 定义要写入的文件中的内容
	char buff[] = "Hello world!\n";

	// 判断文件是否被创建成功
	if(fd < 0){
		printf("create %s is fail!\n", argv[1]);
		return -1;
	}

	// 定义一个打开文件的open函数 O_TRUNC 每一次写文件都会清除再写入
	int wfd = open(argv[1], O_RDWR | O_TRUNC);

	// 创建文件成功，但是根据touch命令的特性，并不需要打开文件，则关闭文件
	printf("create %s is sucess! fd is %d\n", argv[1], fd);

	int len = sizeof(buff);
	printf("len: %d \n", len);

	// 开始写入文件
	wr_ret = write(wfd, buff, sizeof(buff));
	
	printf("value: %d \n", wr_ret);

	// 关闭文件
	close(fd);
	close(wfd);

	return 0;

}
