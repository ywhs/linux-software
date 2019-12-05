#ifndef _CL_M25PXX_H_
#define _CL_M25PXX_H_

#if 0
/**
 *	spi相关：
 *	struct spi_ioc_transfer {
		__u64 tx_buf; 		/* 写数据缓冲 */
		__u64 rx_buf; 		/* 读数据缓冲 */
		__u32 len; 			/* 缓冲的长度 */
		__u32 speed_hz; 	/* 通信的时钟频率 */
		__u16 delay_usecs; 	/* 两个spi_ioc_transfer之间的延时, 单位是微妙 */
		__u8 bits_per_word; /* 字长（比特数） */
		__u8 cs_change; 	/* 是否改变片选 */	
		__u32 pad;
	};
 *	1. mode定义:
	#define SPI_CPHA 0x01 	/* 采样的时钟相位 */
	#define SPI_CPOL 0x02 	/* 时钟信号起始相位：高或者是低电平 */
	#define SPI_MODE_0 (0|0)
	#define SPI_MODE_1 (0|SPI_CPHA)
	#define SPI_MODE_2 (SPI_CPOL|0)
	#define SPI_MODE_3 (SPI_CPOL|SPI_CPHA)
 *	2. 发送比特序，默认高比特在前.
    #define SPI_LSB_FIRST 0x08 /* 发送时低比特在前 */
 ×	
 */
#endif
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdint.h>				// uint32_t等定义
#include <linux/spi/spidev.h>	// spi定义
#include <unistd.h>				// usleep, close等
#include <sys/ioctl.h>			// ioctl()
#include <fcntl.h>				// open，O_RDWR等

// 10M.
#define SPI_DEFAULT_SPEED	10000000
//#define SPI_DBG 
#ifdef SPI_DBG
	#define DBG_FUN_IN	printf(" in:%s:%d\n", __FUNCTION__, __LINE__ );
	#define DBG_FUN_OUT printf("out:%s:%d\n\n", __FUNCTION__, __LINE__ );
	#define DBG(x)		x;
#else
	#define DBG_FUN_IN	
	#define DBG_FUN_OUT 
	#define DBG(x)
#endif

#define M25PXX_CMD_WRITE_ENABLE			0x06
#define M25PXX_CMD_WRITE_DISABLE		0x04
#define M25PXX_CMD_READ_IDENTIFICATION	0x9F
#define M25PXX_CMD_READ_STATUS			0x05
#define M25PXX_CMD_WRITE_STATUS			0x01
#define M25PXX_CMD_READ_BYTES			0x03
#define M25PXX_CMD_PAGE_PROGRAM			0x02
#define M25PXX_CMD_SECTOR_ERASE			0xD8
#define M25PXX_CMD_BULK_ERASE			0xC7


class clm25pxx
{
public:
	// 参数为：spidev. 包括了总线控制器和cs
	// 比如："/dev/spidev0.0"
	clm25pxx( char *spidev = NULL );
	~clm25pxx( );

public:
	int initBus( );

	// 需要执行必要的操作，比如：
	// 在cmd_pagePrograme()中执行等待WIP和执行writeEnable()操作;
	// 在programe结束后，会默认writedisable;
	int write( uint32_t u32Addr, uint8_t* pBuf, uint32_t u32Len )
	{
		int n;
		int ret = u32Len;

		DBG_FUN_IN;
		n = u32Addr % m_u32PageSize;
		if( n != 0 )
		{
			// 不在page边界，此时写: m_u32PageSize - n个;
			n = m_u32PageSize - n;
			n = n > u32Len ? u32Len:n;

			// 开始写
			if( cmd_pageProgram( u32Addr, pBuf, n ) < 0 )
			{
				return -1;
			}
			u32Addr += n;		// 肯定在page边界;
			u32Len -= n;
			pBuf += n;
		}

		// 写多个page.
		while( u32Len )
		{
			n = ( u32Len > m_u32PageSize ) ? m_u32PageSize:u32Len;
			if( cmd_pageProgram( u32Addr, pBuf, n ) < 0 )
			{
				return -1;
			}
			
			u32Addr += n;
			u32Len -= n;
			pBuf += n;
		}
		// 结束!
		DBG_FUN_OUT;
		return ret;
	}

	int read( uint32_t u32Addr, uint8_t* pBuf, uint32_t u32Len )
	{
		int ret;
		DBG_FUN_IN;
		ret = cmd_readBytes( u32Addr, pBuf, u32Len );
		DBG_FUN_OUT;
		return ret;
	}

	int getId( uint8_t au8Id[20])
	{
		int ret;
		DBG_FUN_IN;
		ret = cmd_readIdentification( au8Id );
		DBG_FUN_OUT;
		return ret;
	}

	// 不等待erase结束即返回
	int eraseChip( )
	{
		int ret;
		DBG_FUN_IN;
		ret = cmd_bulkErase();
		DBG_FUN_OUT;
		return ret;
	}

	int setSpiParam( uint8_t u8Mode, uint32_t u32Speed = SPI_DEFAULT_SPEED, uint8_t u8Bits = 8 )
	{
		m_u8Mode 	= u8Mode;
		m_u8Bits 	= u8Bits;
		m_u32Speed 	= u32Speed;	// 可以更高，比如：1MHz之类的.
	}

	// 当前正在编程吗？
	// Program, erase操作，会触发WIP.
	// erase需要时间长，可以查看状态
	int isWIP( )
	{
		uint8_t u8Status;
		cmd_readStatus( &u8Status );
		return u8Status & 0x01;
	}

	int readStatus( uint8_t *pu8Status )
	{
		return cmd_readStatus( pu8Status );
	}
protected:
	// 返回-1，表示在忙
	int waitForWrite( )
	{
		uint8_t u8Status;
		DBG_FUN_IN;

		int wait = 1000;	// 最多 10s
		// 等待
		do
		{
			cmd_readStatus( &u8Status );
			if( ( u8Status & 0x01 ) == 0x00 )
			{
				DBG_FUN_OUT;
				return 1;
			}
			wait--;
			usleep(10000);		// 等待10ms. 该函数单位是us
		}while(wait);

		DBG_FUN_OUT;
		return -1;
	}

	/**
  	 *	基本flash读写函数
	 */

	// write 0x06
	int cmd_writeEnable()
	{
		uint8_t 	u8Cmd = M25PXX_CMD_WRITE_ENABLE;
		int ret;

		DBG_FUN_IN;
		ret = spi_write( &u8Cmd, 1 );
		DBG_FUN_OUT;
		return ret;
	}
	// write 0x04
	int cmd_writeDisable()
	{
		uint8_t 	u8Cmd = M25PXX_CMD_WRITE_DISABLE;
		int ret;
		DBG_FUN_IN;
		ret = spi_write( &u8Cmd, 1 );
		DBG_FUN_OUT;
		return ret;
	}
	// 20字节
	// 0: Manufacturer id
	// 2: device identification
	// 3-19: uid，其中：3: uid的长度, 4-19，实际的uid
	// write: 0x9f, read 20Bytes
	int cmd_readIdentification( uint8_t au8Buf[20] )
	{
		uint8_t u8Cmd = M25PXX_CMD_READ_IDENTIFICATION;
		int ret;

		DBG_FUN_IN;
		// 在ERASE or PROGRAM cycle is in progress
		// 即WIP为1时，不能读; 
		ret = waitForWrite( );
		if( ret < 0 )
		{
			return ret;
		}

		ret = spi_writeAndRead( &u8Cmd, 1, au8Buf, 20 );
		DBG_FUN_OUT;
		return ret;
	}

	// write 0x05, read 1B	
	//  任何时候可以读, status:
	// 	bit 0: WIP, write in progress bit
	//         在PROGRAM, ERASE, WRITE STATUS REGISTER时， set to 1
	// 	bit 1: WEL, write enable latch bit;
	//  bit 4/3/2: BP2/1/0, block protect bits
	//  bit 7: status register write protect

	int cmd_readStatus( uint8_t *pu8status )
	{
		uint8_t u8Cmd = M25PXX_CMD_READ_STATUS;
		int ret;

		DBG_FUN_IN;
		ret = spi_writeAndRead( &u8Cmd, 1, pu8status, 1 );
		DBG_FUN_OUT;
		return ret;
	}

	// write 0x01, read 1B
	// 正常情况下，该函数不用
	int cmd_writeStatus(uint8_t u8status)
	{
		uint8_t 	au8Cmd[2];
		int ret;

		if( cmd_writeEnable( ) < 0 )
		{
			return -1;
		}
		au8Cmd[0] = M25PXX_CMD_WRITE_STATUS;
		au8Cmd[1] = u8status;
		DBG_FUN_IN;
		ret = spi_write( &au8Cmd[0], 2 );
		DBG_FUN_OUT;
		return ret;
	}

	// write: 0x03, add: H8, M8, L8
	// 然后开始读
	int cmd_readBytes( uint32_t u32Addr, uint8_t *pBuf, uint32_t u32Len );

	// write: 0x02, add: H8, M8, L8, dats
	// 注意：最好一次写一个page，即u32Addr L8 = 0, u32Len = u32PageSize, 
	// 否则
	// 	1. 若L8 != 0, 则数据从page内实际地址开始，依次放，到page边界后，返到0x00开始继续;
	//  2. 若u32Len > 256，那么在放满page后，继续返到开始位置，之前的数据会被覆盖;
	// 总之：芯片内，数据存放如同循环队列
	int cmd_pageProgram( uint32_t u32Addr, uint8_t *pBuf, uint32_t u32Len );

	// write: 0xD8, add: H8, M8, L8;
	// 根据sectorid计算add
	// u32SectorId: 从0开始
	int cmd_sectorErase( uint32_t u32SectorId )
	{
		uint8_t au8TxBuf[4];
		uint32_t u32Addr = u32SectorId * m_u32SectorSize;
		int ret;

		DBG_FUN_IN;
		// cmd and addr.
		if( waitForWrite( ) < 0 )
		{
			return -1;
		}
		cmd_writeEnable( );

		au8TxBuf[0] = M25PXX_CMD_SECTOR_ERASE;
		au8TxBuf[1] = (u32Addr >> 16) & 0xFF;
		au8TxBuf[2] = (u32Addr >> 8) & 0xFF;
		au8TxBuf[3] = u32Addr & 0xFF;

		ret = spi_write( &au8TxBuf[0], 4);
		DBG_FUN_OUT;
		return ret;
	}

	// write: 0xC7
	int cmd_bulkErase( )
	{
		uint8_t u8Cmd = M25PXX_CMD_BULK_ERASE;
		int ret;
		DBG_FUN_IN;
		if( waitForWrite( ) < 0 )
		{
			return -1;
		}
		cmd_writeEnable( );
		ret = spi_write( &u8Cmd, 1 );
		DBG_FUN_OUT;
		return ret;
	}

	// 基本总线操作
	int spi_write( uint8_t *pBuf, uint32_t u32Len );
	int spi_writeAndRead( uint8_t *pWrBuf, uint32_t u32WrLen, 
				uint8_t *pRdBuf, uint32_t u32RdLen );
protected:
	// spi bus
	char 	m_spidev[100];
	int 	m_fd;					// 设备文件描述符
	struct spi_ioc_transfer m_tf[2];	// 写wr
	uint8_t 	m_u8Mode;
	uint8_t		m_u8Bits;
	uint32_t	m_u32Speed;

	// 缓冲区，用于programe
	uint8_t*	m_pTxBuf;

	// flash特性
	uint32_t m_u32PageSize;	// page size
	uint32_t m_u32SectorSize;	// sector size
	uint32_t m_u32ChipSize;	// chip size
};

// m25p80的最高速率可达：20MHz之上
// 其clk 最高75/25 MHz，采用的读自己模式 READ BYTES最该 33/20MHz
// 设置为10MHz
// rpi: 最高支持到125MHz, 因此默认为10MH

class clm25p80 : public clm25pxx
{
public:
	clm25p80( char *spidev = NULL )
		: clm25pxx( spidev )
	{
		m_u32PageSize 	= 256;			// 256 bytes
		m_u32SectorSize = 65536;		// 256 page
		// 最后一个sector的范围为：0xF,0000,-0xF,EFFF，少1K
		// 最后一个sector不计入总存贮空间吧
		m_u32ChipSize 	= 1024*1024;	// 16 sectors
		
		m_u8Bits = 8;
		m_u8Mode = SPI_MODE_3;	// mode 0 或者 3都可以.
		m_u32Speed = SPI_DEFAULT_SPEED;
	}
};

#endif

