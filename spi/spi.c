/*******************************************	author: ywh*	data: 19-10-28*	desc: 在应用层对spi设备进行读写 实际上流程比较简单：打开设备文件 -> 配置设备 -> 读写设备 -> 关闭设备文件。******************************************/#include <fcntl.h>#include <unistd.h>#include <sys/ioctl.h>#include <linux/types.h>#include <linux/spi/spidev.h>#include "../common/common.h"

int spi_write(){	// 定义文件变量
	int fd;
	// 打开spi设备
	fd = open(SPI_DEV, O_WRONLY);

	printf("fd value is : %d \n", fd);
                            
  	return 1;}
int main(){
	spi_write();

	return 1;}

