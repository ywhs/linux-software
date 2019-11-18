#include "clm-copy.h"

using namespace std;


/* 实现clm-copy.h中没有实现的函数 */

/* 实现有参构造函数 */
clm25pxx :: clm25pxx( const char *spi_dev )
{
	printf("有参构造函数 \n");
	strcpy( this->spi_dev, spi_dev );
	printf("spidev: %s", this->spi_dev);

}


/* 实现无参构造函数 */
clm25pxx :: clm25pxx()
{
	printf("无参构造函数 \n");
	if( init() < 0 ){
		printf("初始化失败！\n");
	}
}

/* 初始化函数 */
int clm25pxx :: init()
{

	printf("初始化函数 \n");
	strcpy( spi_dev, "/dev/spidev0.0" );
	/* 打开设备 */
	fd = open(spi_dev, O_RDWR);
	if( fd < 0 ){
		pabort("can't open device");
		return -1;
	}

	/* 设置mode */
	m_u8Mode = SPI_MODE_0;
	if( ioctl(fd, SPI_IOC_WR_MODE, &m_u8Mode) < 0 ){
		pabort("can't set spi mode");
		return -2;
	}

	/* 设置bit字节 */
	m_u8Bits = 8;
	if( ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &m_u8Bits) < 0 ){
		pabort("can't set bits per word");
		return -3;
	}

	/* 设置速率 */
	m_u32Speed = SPI_DEFAULT_SPEED;
	if( ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &m_u32Speed) < 0 ){
		pabort("can't set max speed hz");
		return -4;
	}

	printf("spi mode: %d\n", m_u8Mode);
	printf("bits per word: %d\n", m_u8Bits);
	printf("max speed: %d Hz (%d KHz)\n", m_u32Speed, m_u32Speed/1000);
	printf("device : %s \n", spi_dev);
	return 1;
}

/* 获取设备id */
int clm25pxx :: get_id(uint8_t read_buff[])
{

	printf("获取设备id \n");
	
	/* m25p80中规定读取id的指令 */
	uint8_t write_buff[1] = {M25P80_READ_IDENTIFICATION};
	
	return write_and_read( write_buff, read_buff, 1, 20 );

}

/* 实现读取数据 */
int clm25pxx :: read_data( uint32_t addr, uint8_t read_buff[], uint32_t re_len )
{	
	/* 定义写数组 */
	uint8_t write_buff[4];
	printf("实现读取数据 \n");
	/* 赋值读数据指令和地址 */
	write_buff[0] = M25P80_READ_DATA_BYTES;
	/* addr 传进来的地址虽然是放到了数组中的高位上，但是实际存的是时序上的低位地址*/
	write_buff[1] = (addr >> 16) & 0xFF;
	write_buff[2] = (addr >> 8) & 0xFF;
	write_buff[3] = addr & 0xFF;

	printf("addr: 0x%.2x \n", write_buff[1]);
	printf("addr: 0x%.2x \n", write_buff[2]);
	printf("addr: 0x%.2x \n", write_buff[3]);

	
	return write_and_read(write_buff, read_buff, 4, re_len);
}

/* 实现写和读的功能 */
int clm25pxx :: write_and_read( uint8_t write_buff[], uint8_t read_buff[], uint32_t wr_len, uint32_t re_len )
{	
	
	/* 为数组进行初始化,数组如果不进行初始化的话就会给你赋值随机数，好像如果定义在全局的话就不是随机数 */
	memset(m_tf, 0x00, sizeof(m_tf));

	/* 为结构体中变量赋值 */
	m_tf[0].tx_buf = (unsigned long)write_buff;	
	m_tf[0].rx_buf = 0;
	m_tf[0].len = wr_len;
	m_tf[0].speed_hz = m_u32Speed;
	m_tf[0].bits_per_word = m_u8Bits;

	m_tf[1].tx_buf = 0;
	m_tf[1].rx_buf = (unsigned long)read_buff;
	m_tf[1].len = re_len;
	m_tf[1].speed_hz = m_u32Speed;
	m_tf[1].bits_per_word = m_u8Bits;

	/* 开始读取数据或写入数据 */
	if( ioctl(fd, SPI_IOC_MESSAGE(2), m_tf) < 0 ){
		pabort("can't send spi message");
		return -1;
	}

	return 1;
}


/* 实现析构函数 */
clm25pxx :: ~clm25pxx()
{

	printf("析构函数！ \n");
	close(fd);

}







