/* 引入几个常用的头文件 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* 定义spi设备常量 */
#define SPI_DEV ("/dev/spidev0.0") 

/* 获取数组的大小      ，通过获取数组的内存总大小除以一个元素的内存大小，就是元素个数  */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* 设备模式 */
uint8_t mode = SPI_MODE_3;
/* 字长 */
uint8_t bits = 8;
/* 最大时钟频率 */
uint32_t speed = 50*1000;


/**
* 声明函数
**/

/* 打印调试信息 */
bool debug_msg(int result, const char *str);
bool debug_msg(int result, const char *str)
{
	if(result < 0){
		printf("err: %s 失败! \n", str);
		exit(1);
	}
	return true;
}
static void spi_init();
static int spi_write();
static int spi_read();


