#include <stdio.h>
#include "clm-copy.h"

int main(int argc, char *argv[]){


	char *spidev = (char *)"/dev/spidev0.0";
	
	uint8_t read_id_buff[20];	

	/* 调用无参函数 */
	clm25pxx clm;
	//clm25pxx clm(spidev);
// 调用获取设备id
#if 1
	// 第一个参数为：获取数据的数组
	clm.get_id(read_id_buff);

	for( int i = 0; i < 20; i++ )
	{
		printf("%2x ", read_id_buff[i]);
	}
#endif
	puts("");

// 调用获取数据	
#if 1
	uint8_t read_data_buff[20];
	// read_data函数的第一个参数为: 从哪个地址开始读取
	// 第二个参数为：获取数据的数组
	// 第三个参数为：获取数据的数组的长度
	clm.read_data(0, read_data_buff, 20 );


	for( int i = 0; i < 20; i++ )
	{
		printf("0x%.2x ", read_data_buff[i]);
	}
	puts("");
#endif	


	return 1;

}
