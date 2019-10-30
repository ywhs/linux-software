# 应用层对spi设备进行读写操作

SPI总线设备文件名通常为/dev/spidevN.P（N=0、1、2……，P=0、1、2……），其中N表示第几路SPI总线，而P表示在该路SPI总线中使用哪个CS信号线。

一般在**linux**的源码**linux/Documentation/spi**路径下有关于spi的介绍，我参考了**linux/Documentation/spi/spidev**文件，文件中介绍了应该使用哪些头文件

```bash
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
```

## spidev.h

在上面的spidev.h中有着我们编程时用到的结构体

路径：**linux/Documentation/spi/spidev**

```bash
struct spi_ioc_transfer {
	__u64		tx_buf;   	//发送缓存区
	__u64		rx_buf;   	//接收缓存区

	__u32		len;		// 这个长度我也不太确定，实验后，在进行写操作时，我设置为多少并没有影响，而在读时如果设置为7的话，数组前7位的值都为0，从第8位才可以读取到值
	__u32		speed_hz;	// 总线速率

	__u16		delay_usecs;	// 发送时间间隔
	__u8		bits_per_word;	//收发的一个字的二进制位数
	__u8		cs_change;		// 指定这个cs_change结束之后是否需要改变片选线
	__u8		tx_nbits;
	__u8		rx_nbits;
	__u16		pad;

	/* If the contents of 'struct spi_ioc_transfer' ever change
	 * incompatibly, then the ioctl number (currently 0) must change;
	 * ioctls with constant size fields get a bit more in the way of
	 * error checking than ones (like this) where that field varies.
	 *
	 * NOTE: struct layout is the same in 64bit and 32bit userspace.
	 */
}

```

## 对spi设备的初始化

在对spi设备进行读写之前需要对其初始化，也就是规定如何工作之类的

```bash

	uint8_t mode = SPI_MODE_3;
	/* 字长 */
	uint8_t bits = 8;
	/* 最大时钟频率 */
	uint32_t speed = 50*1000;
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

```

上述代码中的 **SPI_IOC_WR_MODE** 类似这种也是在 **linux/Documentation/spi/spidev** 中定义的，规定了都需要初始化什么

### 详细介绍

- 用户空间对spidev设备节点使用IOCTL命令失败会返回-1。

- SPI_IOC_RD_MODE

```
读取SPI设备对应的spi_device.mode，使用的方法如下：
ioctl(fd,SPI_IOC_RD_MODE, &mode);
其中第三个参数是一个uint8_t类型的变量
```
- SPI_IOC_WR_MODE

```
设置SPI设备对应的spi_device.mode。使用的方式如下：
ioctl(fd,SPI_IOC_WR_MODE, &mode);
```

- SPI_IOC_RD_LSB_FIRST

```
查看设备传输的时候是否先传输低比特位。如果是的话，返回1。使用的方式如下：
ioctl(fd,SPI_IOC_RD_LSB_FIRST, &lsb);
其中lsb是一个uint8_t类型的变量。返回的结果存在lsb中。
```

- SPI_IOC_WR_LSB_FIRST

```
设置设备传输的时候是否先传输低比特位。当传入非零的时候，低比特在前，当传入0的时候高比特在前（默认）。使用的方式如下：
ioctl(fd,SPI_IOC_WR_LSB_FIRST, &lsb);
```

- SPI_IOC_RD_BITS_PER_WORD

```
读取SPI设备的字长。使用的方式如下：
ioctl(fd,SPI_IOC_RD_BITS_PER_WORD, &bits);
其中bits是一个uibt8_t类型的变量。返回的结果保存在bits中。
```

- SPI_IOC_WR_BITS_PER_WORD

```
设置SPI通信的字长。使用的方式如下：
ioctl(fd,SPI_IOC_WR_BITS_PER_WORD, &bits);
```


- SPI_IOC_RD_MAX_SPEED_HZ

```
读取SPI设备的通信的最大时钟频率。使用的方式如下：
ioctl(fd,SPI_IOC_RD_MAX_SPEED_HZ, &speed);
其中speed是一个uint32_t类型的变量。返回的结果保存在speed中。
```

- SPI_IOC_WR_MAX_SPEED_HZ

```
设置SPI设备的通信的最大时钟频率。使用的方式如下：
ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ, &speed);
```

# 对spi设备进行读写

对设备进行初始化以后就可以对设备读写，先定义结构体在读写，具体看代码吧

- 写操作

```bash
	unsigned char tx[20];
	int ret, i;
	tx[0] = 0x01;
	// 填充数组
	for(i = 1; i < 20; i++){
		tx[i] = tx[i-1] + 1;
	}
	
	// 定义结构体
	struct spi_ioc_transfer write_tr = {
		.tx_buf = (unsigned long)tx, //发送缓存区
		.len = 20,
		.speed_hz = speed, //总线速率
		.bits_per_word = bits, //收发的一个字的二进制位数
	};
	
	// 对spi设备写操作, SPI_IOC_MESSAGE 这个宏定义也是在spidev文件中定义的
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &write_tr);
	if(ret < 0)
		debug_msg(ret, "写入数据");
```


- 读操作

```bash
	unsigned char rx[20];
	int ret;
	
	// 定义结构体，如果是读写操作在一个函数中的话定义一个结构体即可
	struct spi_ioc_transfer read_tr = {
		.rx_buf = (unsigned long)rx, //接收缓存区
		.len = 0,
		.speed_hz = speed, //总线速率
		.bits_per_word = bits, //收发的一个字的二进制位数
	};
	
	// 对spi设备读操作
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &read_tr);
	if(ret < 0)
		debug_msg(ret, "读取数据");
	
	// 输出读到数据
	for(int i = 0; i < 20; i++){
		printf("value is : 0x%.2X \n", rx[i]);
	}

```

可以先进行写操作在进行读操作，看一下读出来的数据是不是写进去的数据即可，还可以具体阅读myself-spi.c文件

# 参考

- [spi-dev-rw](https://emcraft.com/stm32f429discovery/accessing-spi-devices-in-linux)

- [用户空间的spi驱动](https://blog.csdn.net/yuanlulu/article/details/6320740)
















