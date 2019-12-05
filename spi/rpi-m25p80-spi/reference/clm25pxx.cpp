#include "clm25pxx.h"


clm25pxx::clm25pxx( char *spidev )
{
	if( spidev ) 
	{
		strcpy( m_spidev, spidev );
	}
	else
	{
		strcpy( m_spidev, "/dev/spidev0.0" );
	}
	m_fd = -1;
	m_u8Mode 	= SPI_MODE_3;
	m_u8Bits 	= 8;
	m_u32Speed 	= SPI_DEFAULT_SPEED;	// 默认：10 MHz之类的.

	// 默认参数
	m_u32PageSize 	= 256;			// 256 bytes
	m_u32SectorSize = 65536;		// 256 page
	m_u32ChipSize 	= 1024*1024;	// 8MBits

	m_pTxBuf = NULL;
}

clm25pxx::~clm25pxx( )
{
	if( m_pTxBuf )
	{
		delete[] m_pTxBuf;
	}

	if( m_fd != -1 )
	{
		close(m_fd);
	}
}

int clm25pxx::initBus()
{
	// 测试表明，每次open()，设备参数恢复到默认状态，比如speed为125MHz
	m_fd = open(m_spidev, O_RDWR);
	if (m_fd < 0)
	{
		return -1;
	}

	/*
	 * spi mode
	 * SPI_IOC_WR_MODE, 设置mode
  	 * SPI_IOC_RD_MODE, 获取mode
	 */
	if( ioctl(m_fd, SPI_IOC_WR_MODE, &m_u8Mode) < 0 )
	{
		return -2;	// "can't set spi mode"
	}

	/**
	 * max speed hz
	 * SPI_IOC_WR_MAX_SPEED_HZ, 设置
 	 * SPI_IOC_RD_MAX_SPEED_HZ, 获取
	 */
	
	if( ioctl(m_fd, SPI_IOC_WR_MAX_SPEED_HZ, &m_u32Speed) < 0 )
	{
		return -3;	// "can't set max speed hz"
	}

	/**
	 * bits per word
  	 * SPI_IOC_WR_BITS_PER_WORD, 设置
  	 * SPI_IOC_RD_BITS_PER_WORD, 读取
 	 * 默认：8bits
	 */
	if( ioctl(m_fd, SPI_IOC_WR_BITS_PER_WORD, &m_u8Bits) < 0 )
	{
		return -4;	// "can't set bits per word"
	}

	/**
  	 *	比特序号不设置，高比特在前.
	 *  SPI_IOC_WR_LSB_FIRST, 设置, 非0, LSB first, 0表示 MSB first
     *	SPI_IOC_RD_LSB_FIRST, 获取
 	 *  ioctl(fd, SPI_IOC_WR_LSB_FIRST, &lsb);
	 *	不必设置，默认为0表示高比特在前
	 */

	memset( (void *)&m_tf[0], 0x00, sizeof(m_tf[0]));
	memset( (void *)&m_tf[1], 0x00, sizeof(m_tf[1]));
	// 应该可以不赋值：已经设置过了
//	m_tf[0].speed_hz = m_tf[1].speed_hz = m_u32Speed;
//	m_tf[0].bits_per_word = m_tf[1].bits_per_word = m_u8Bits;

	m_pTxBuf = new uint8_t[4+m_u32PageSize];
	if( !m_pTxBuf )
	{
		return -5;
	}
	return 1;
}

int clm25pxx::spi_write( uint8_t *pBuf, uint32_t u32Len )
{
	int ret;

	DBG_FUN_IN;
	if( m_fd < 0 )
	{
		return -1;
	}

	m_tf[0].tx_buf = (uint64_t)pBuf;
	m_tf[0].rx_buf = 0;
	m_tf[0].len 	= u32Len;
	ret = ioctl(m_fd, SPI_IOC_MESSAGE(1), &m_tf[0]);

	DBG_FUN_OUT;
	return ret;
}

int clm25pxx::spi_writeAndRead( uint8_t *pWrBuf, uint32_t u32WrLen, 
				uint8_t *pRdBuf, uint32_t u32RdLen )
{
	int ret;

	DBG_FUN_IN;
	if( m_fd < 0 )
	{
		return -1;
	}

	m_tf[0].tx_buf = (uint64_t)pWrBuf;
	m_tf[0].rx_buf = 0;
	m_tf[0].len 	= u32WrLen;

	m_tf[1].tx_buf = 0;
	m_tf[1].rx_buf = (uint64_t)pRdBuf;
	m_tf[1].len 	= u32RdLen;

	ret = ioctl(m_fd, SPI_IOC_MESSAGE(2), m_tf);	
	DBG_FUN_OUT;
	return ret;
}

	// 通过spi总线，write/read
int clm25pxx::cmd_readBytes( uint32_t u32Addr, uint8_t *pBuf, uint32_t u32Len )
{
	uint8_t au8TxBuf[4];
	int ret;

	DBG_FUN_IN;
	// 当status register中，WIP=1，即芯片正在执行写操作时，不能读
	ret = waitForWrite( );
	if( ret < 0 )
	{
		return ret;
	}
	// cmd and addr.
	au8TxBuf[0] = M25PXX_CMD_READ_BYTES;
	au8TxBuf[1] = (u32Addr >> 16) & 0xFF;
	au8TxBuf[2] = (u32Addr >> 8) & 0xFF;
	au8TxBuf[3] = u32Addr & 0xFF;

	ret = spi_writeAndRead( au8TxBuf, 4, pBuf, u32Len );
	DBG_FUN_OUT;
	return ret;
}

// 不考虑业务合法性
int clm25pxx:: cmd_pageProgram( uint32_t u32Addr, uint8_t *pBuf, uint32_t u32Len )
{
	int ret;
	DBG_FUN_IN;

	if( u32Len > m_u32PageSize )
	{
		return -1;
	}
	ret = waitForWrite( );
	if( ret < 0 )
	{
		return ret;
	}
	cmd_writeEnable( );

	// cmd and addr.
	m_pTxBuf[0] = M25PXX_CMD_PAGE_PROGRAM;
	m_pTxBuf[1] = (u32Addr >> 16) & 0xFF;
	m_pTxBuf[2] = (u32Addr >> 8) & 0xFF;
	m_pTxBuf[3] = u32Addr & 0xFF;
	
	memcpy( m_pTxBuf+4, pBuf, u32Len );
	ret = spi_write( m_pTxBuf, 4+u32Len );
	DBG_FUN_OUT;
	return ret;
}


