#ifndef _CL_COPY_H_
#define _CL_COPY_H_

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdint.h>				// uint32_t等定义
#include <linux/spi/spidev.h>	// spi定义
#include <unistd.h>				// usleep, close等
#include <sys/ioctl.h>			// ioctl()
#include <fcntl.h>

#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <linux/types.h>

/* 读取设备id指令 */
#define M25P80_READ_IDENTIFICATION		0x9F
#define M25P80_READ_DATA_BYTES			0x03


// 10M.
#define SPI_DEFAULT_SPEED	10000000
#ifndef SPI_DBG
	#define DBG_FUN_IN	printf(" in:%s:%s:%d\n", __FUNCTION__, __LINE__ );
	#define DBG_FUN_OUT printf("out:%s:%s:%d\n", __FUNCTION__, __LINE__ );
	#define DBG(x)		x;
#else
	#define DBG_FUN_IN	
	#define DBG_FUN_OUT 
	#define DBG(x)
#endif



/* 定义输出函数 */
static void pabort(const char *s)
{
	perror(s);
	abort();
}



/* 定义一个基类，和java一样 class 类名 */
class clm25pxx
{

/* 定义成员变量 */
protected:

	/* spi设备 带片选 */
	char 		spi_dev[100];
	/* 设备描述符 */
	int 		fd;
	/* 读写设备的结构体 */
	struct 		spi_ioc_transfer m_tf[2];
	/* spi设备的模式 */
	uint8_t 	m_u8Mode;
	/* 设备的字节数 */
	uint8_t		m_u8Bits;
	/* 设备速率 */
	uint32_t	m_u32Speed;


/* 定义构造函数和析构函数 */
public:
	/* 有参构造函数 */
	clm25pxx( const char *spi_dev );
	/* 无参构造函数 */
	clm25pxx( );
	/* 析构函数，销毁函数 */
	~clm25pxx( );

/* 定义向外提供的接口 */
public:

	/* 
		desc: 获取设备id
		read_buff: 读取数据的数组，读取后会返回到此数组中
	*/
	int get_id(   uint8_t read_buff[] );

	/* 
		desc: 读取flash数据
		read_buff[]: 读取数据的数组，读取后会返回到此数组中
		re_len: 读取的长度，跟读取数据的数组长度一致
	*/
	int read_data( uint32_t addr, uint8_t read_buff[], uint32_t re_len );

/* 定义保护函数，自己和子类能调用 */
protected:

	/* 初始化spi设备函数 */
	int init();
	/* 基本写和读的功能 */
	int write_and_read( uint8_t write_buff[], uint8_t read_buff[], uint32_t wr_len, uint32_t re_len );

	


};

/* 子类继承父类 语法：class derived-class: access-specifier base-class */
class clm25p80 : public clm25pxx
{

/* 定义构造函数 */
public:
	
};


#endif



