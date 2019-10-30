/******************************************
 *	author: ywh
 *	data: 19-10-28
 *	desc: 在应用层对spi设备进行读写 实际上流程比较简单：打开设备文件 -> 配置设备 -> 读写设备 -> 关闭设备文件。
 *			SPI总线设备文件名通常为/dev/spidevN.P（N=0、1、2……，P=0、1、2……），其中N表示第几路SPI总线，而P表示在该路SPI总线中使用哪个CS信号线
 *******************************************/
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "myself-spi.h"

/* 文件设备的句柄 */
static int fd;

/* 函数如果放在man函数之前不需要声明，如果放在main函数之后需要先声明函数 */
int main(int argc, char * argv[]){

	int ret;
	bool flag = true;
	/* 获取脚本参数，根据不同参数执行不同操作  ,以下带参数的方法还没有写，到时候根据具体的需求修改带参的方法 */
	while((ret = getopt(argc,argv,"r::w::")) != -1){
		switch(ret){
			case 'r':
				flag = false;
				spi_init();
				spi_read();
				break;
			case 'w':
				flag = false;
				spi_init();
				spi_write();
				break;
			default:
				printf("nothing! \n");
				break;
		}
	}


	if(flag){
		spi_init();
		spi_write();
		spi_read();
	}

	/* 关闭设备文件 */
	close(fd);
	
	return 1;
}



/* 初始化spi static关键字的作用是 使spi_init函数只在本文件能被调用，就是作用域的问题，相当于java中的private关键字 */
static void spi_init(){
	int ret;
	
	// 1、打开spi设备
	fd = open(SPI_DEV, O_RDWR);
	if(fd < 0)
		debug_msg(fd, "打开spi设备");

	/*	spi mode 设置读写模式 */
	
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if(ret < 0)
		debug_msg(fd, "设置写设备模式");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if(ret < 0)
		debug_msg(fd, "设置读设备模式");

	/* spi word length 字长  1到N*/
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if(ret < 0)
		debug_msg(fd, "设置写字长");
	
	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if(ret < 0)
		debug_msg(fd, "设置读字长");

	/* max speed hz 最大时钟频率 */

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if(ret < 0)
		debug_msg(fd, "设置写最大时钟频率");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if(ret < 0)
		debug_msg(fd, "设置读最大时钟频率");

	/* 设置传输的时候先传输低位还是高位，使用默认即可 */


}

/* spi 写操作 */
static int spi_write(){	
	
	unsigned char tx[20];
	int ret, i;
	tx[0] = 0x01;
	for(i = 1; i < 20; i++){
		tx[i] = tx[i-1] + 1;
	}

	struct spi_ioc_transfer write_tr = {
		.tx_buf = (unsigned long)tx, //发送缓存区
		.len = 20,
		.speed_hz = speed, //总线速率
		.bits_per_word = bits, //收发的一个字的二进制位数
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &write_tr);
	if(ret < 0)
		debug_msg(ret, "写入数据");

  	return 1;
}

/* spi读取操作 */
static int spi_read(){	
	
	unsigned char rx[20];
	int ret;

	struct spi_ioc_transfer read_tr = {
		.rx_buf = (unsigned long)rx, //接收缓存区
		.len = 0,
		.speed_hz = speed, //总线速率
		.bits_per_word = bits, //收发的一个字的二进制位数
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &read_tr);
	if(ret < 0)
		debug_msg(ret, "读取数据");

	for(int i = 0; i < 20; i++){
		printf("value is : 0x%.2X \n", rx[i]);
	}
	
	
  	return 1;
}







