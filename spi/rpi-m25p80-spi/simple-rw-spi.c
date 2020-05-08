/******************************************
 *	author: ywh
 *	data: 19-10-28
 *	desc: 在应用层对spi设备进行读写 实际上流程比较简单：打开设备文件 -> 配置设备 -> 读写设备 -> 关闭设备文件。
 *			SPI总线设备文件名通常为/dev/spidevN.P（N=0、1、2……，P=0、1、2……），其中N表示第几路SPI总线，而P表示在该路SPI总线中使用哪个CS信号线
 *******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/stat.h>         // 读取linux中的文件状态
#include "clm25pxx.h"


#define BUF_LEN	4096
#define SUCCESS 1
#define FAILE	-1
#define FILEPATH "/etc/orio/fpga"


/* 比较两个文件是否一致 */
int file_compare(const char *source_filename, const char *target_filename){
		
	FILE *s_fp, *t_fp;
	uint8_t s_read_data[BUF_LEN], t_read_data[BUF_LEN];
	int s_ret = 0, t_ret = 0, len = 0;


	s_fp = fopen(source_filename, "r");
	t_fp = fopen(target_filename, "r");

	if(NULL == s_fp && NULL == t_fp){
		printf("file is not exist！ \n");
		return FAILE;
	}

	// 只要有一个到了文件结尾就退出
	while(!feof(s_fp) && !feof(t_fp)){

		s_ret = fread(s_read_data, sizeof(s_read_data[0]), BUF_LEN, s_fp);
		t_ret = fread(t_read_data, sizeof(t_read_data[0]), BUF_LEN, t_fp);

		// 以文件小的长度比较
		len = s_ret < t_ret ? s_ret : t_ret;

		for(int i = 0; i < len; i++){
			if(s_read_data[i] != t_read_data[i]){
				fclose(s_fp);
				fclose(t_fp);
				printf("File content is different \n");
				return FAILE;
			}
		}
	}

	printf("File content is the same \n");

	return SUCCESS;

}

/* 读取fpga内容到文件 */
int spi_read_data_to_file(const char *filename, uint32_t read_start_address, uint32_t read_end_address){

	clm25p80 m25p80;
	FILE *fp;
	uint8_t read_data[BUF_LEN];
	int n = 0, ret = 0;

	// 初始化设备
	if( (ret = m25p80.initBus()) < 0 )
	{
		printf("init device err, err_code = %d \n", ret);
	}
	
	fp = fopen( filename, "wb" );
	while( read_start_address < read_end_address){
		n = read_end_address - read_start_address < BUF_LEN ? (read_end_address - read_start_address) : BUF_LEN;
		// 从fpga中读取
		if( (ret = m25p80.read( read_start_address, read_data, n )) < 0 ){
			printf("read err, err_code = %d \n", ret);
			return FAILE;
		}
		fwrite( read_data, sizeof(read_data[0]), n, fp);
		read_start_address += n;
		printf("..");
	}

	fclose(fp);

	puts("");
	return SUCCESS;
}


/* spi 写fpga操作 */
int spi_write_data(char *filename){

	// 写数组
	uint8_t 	write_data[BUF_LEN] = {0x00};
	// 默认从0开始写入
	uint32_t 	write_address = 0;
	// 返回结果变量
	int ret = 0, second = 0;
	// 读取的文件指针变量
	FILE *fp;
	clm25p80 m25p80;

	printf("write data \n");

	fp = fopen(filename, "r");

	// 判断文件是否存在
	if(NULL == fp)	{
		printf("file is not exist！");
		fclose(fp);
		return FAILE;
	}

	// 初始化设备
	if( (ret = m25p80.initBus()) < 0 )
	{
		printf("init device err, err_code = %d \n", ret);
		return FAILE;
	}

	// 擦除fpga空间
	if( m25p80.eraseChip() < 0 ){
		printf("erase err！ \n");
		return FAILE;
	}

	// 判断是否还在擦除操作,擦除操作大概8秒钟
	while(1)
	{
		if( !m25p80.isWIP( ) )
		{
			break;
		}
		printf("programing... %ds\n", second++);
		usleep(1000000);
	}

	// 读取数据并写入fpga
	while(1){

		// 读入到数组中
		ret = fread(write_data, sizeof(write_data[0]), BUF_LEN, fp);
		// 判断是否读取完毕
		if(ret == 0){
			printf("end of file input! \n");
			printf("file size: %d\n", write_address);
			fclose(fp);
			break;
		}else if(ret < 0){
			printf("write data err! \n");
			fclose(fp);
			return FAILE;
		}

		/* 
			1.从哪个地址开始
			2.写入的数据
			3.数据的长度
			desc:写入fpga中 
		*/
		if( m25p80.write( write_address, write_data, ret ) < 0 ){
			printf("write err! \n");
			return FAILE;
		}

		printf("..");

		// 基地址 + 偏移地址,地址前移
		write_address += ret;

	}

	puts("");

	
	if(spi_read_data_to_file(FILEPATH, 0, write_address) < 0){
		printf("write fpga data to file err! \n");
		return FAILE;
	}

	// 判断写入fpga的内容和文件内容是否一致
  	return file_compare(filename, FILEPATH);
}

void Usage(){

	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("\n");
	printf(" -f [file_path] write file content to fpga \n");
	printf(" -h: help \n");
	printf("\n");
	printf(" For example: ./dl-spi -f filename \n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	exit(1);
}


/* 函数如果放在man函数之前不需要声明，如果放在main函数之后需要先声明函数 */
int main(int argc, char * argv[]){

	bool flag = false;
	int ret = 0;

	/* 获取脚本参数，根据不同参数执行不同操作 */
	while((ret = getopt(argc,argv,"f:h")) != -1){
		switch(ret){
			case 'f':
				flag = true;
				spi_write_data(optarg);
				break;
			case 'h':
				Usage();
				break;
		}
	}

	if(!flag){
		Usage();
	}
	
	return 1;
}