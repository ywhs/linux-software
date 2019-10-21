/**
	ALSA音频应用层编程
	auth:	ywh
	date:	2019-9-4
	idea:	先打开一个普通wav音频文件，从定义的文件头前面的44个字节中，取出文件头的定义消息，置于一个文件头的结构体中。
			然后打开alsa音频驱动，从文件头结构体取出采样精度，声道数，采样频率三个重要参数，利用alsa音频驱动的API设置好参数，
			最后打开wav文件，定位到数据区，把音频数据依次写到音频驱动中去，开始播放，当写入完成后，退出写入的循环。
	rel:	- [参考的想法](https://www.bbsmax.com/A/n2d9x2D05D/)
			- [alsa的函数库](https://www.alsa-project.org/alsa-doc/alsa-lib/group___p_c_m.html#ga8340c7dc0ac37f37afe5e7c21d6c528b)
**/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <alsa/asoundlib.h>
#include "const.h"


int main(int argc, char *argv [])
{
	
	// 判断命令参数中是携带音乐文件
	if(argc != 2){
		printf("err: not found music file \n");
		exit(1);
	}
	// 打开音乐文件
	open_music_file(argv[1]);

	// 1. 在堆栈上分配snd_pcm_hw_params_t结构体的空间，参数是配置pcm硬件的指针,返回0成功
	is_set(snd_pcm_hw_params_malloc(&hw_params), "分配snd_pcm_hw_params_t结构体");

	// 2. 打开PCM设备 返回0 则成功，其他失败
	// 函数的最后一个参数是配置模式，如果设置为0,则使用标准模式
	// 其他值位SND_PCM_NONBLOCL和SND_PCM_ASYNC 如果使用NONBLOCL 则读/写访问, 如果是后一个就会发出SIGIO（没太懂什么意思）
	pcm_name = strdup("hw:0,0");
	is_set(snd_pcm_open(&pcm_handle, "default", stream, 0), "打开PCM设备");

	// 3. 在我们将PCM数据写入声卡之前，我们必须指定访问类型，样本格式，采样率，通道数，周期数和周期大小。
	// 首先，我们使用声卡的完整配置空间初始化hwparams结构
	is_set(snd_pcm_hw_params_any(pcm_handle, hw_params), "配置空间初始化");
 
	// 3.1设置交错模式（访问模式）
	// 常用的有 SND_PCM_ACCESS_RW_INTERLEAVED（交错模式） 和 SND_PCM_ACCESS_RW_NONINTERLEAVED （非交错模式） 参考：https://blog.51cto.com/yiluohuanghun/868048
	is_set(snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED), "设置交错模式（访问模式）");

	// 3.2 设置采样位数 根据音乐文件返回的采样位数设置
	if(wav_header.bits_per_sample == 8){
		pcm_format = SND_PCM_FORMAT_S8;
	}else if(wav_header.bits_per_sample == 16){
		pcm_format = SND_PCM_FORMAT_S16_LE;
	}
	is_set(snd_pcm_hw_params_set_format(pcm_handle, hw_params, pcm_format), "设置采样位数");

	// 3.3 设置采样率为44.1KHz dir的范围(-1,0,1)
	// rate = wav_header.sample_rate;
	int dir = 0;
	is_set(snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &wav_header.sample_rate, &dir), "设置采样率");

	// 3.4 设置通道数
	is_set(snd_pcm_hw_params_set_channels(pcm_handle, hw_params, wav_header.num_channels), "设置通道数");

	// 4. 设置的硬件配置参数，加载，并且会自动调用snd_pcm_prepare()将stream状态置为SND_PCM_STATE_PREPARED
	is_set(snd_pcm_hw_params(pcm_handle, hw_params), "设置的硬件配置参数");
	// 释放之前配置的参数
	snd_pcm_hw_params_free(hw_params);

	
	// feof函数检测文件结束符，结束：非0, 没结束：0
	int ret = fread(buf, 1, BUF_LEN, fp);
	printf("value is : %d \n", ret);
	while(!feof(fp)){
		int ret = fread(buf, 1, BUF_LEN, fp);
		if(ret == 0){
			is_set(-1, "end of music file input! \n");
		}
		if(ret < 0){
			is_set(-1, "read pcm from file");
		}
		// 向PCM设备写入数据
		ret = snd_pcm_writei(pcm_handle, buf, BUF_LEN/4);
		if(ret < 0){
			is_set(-1, "write to audio interface failed");
		}
	}

	fprintf(stderr, "end of music file input\n");
	
	// 关闭文件
	fclose(fp);
	snd_pcm_close(pcm_handle);
	
	return 0;

}

// 想要定义函数必须要先声明函数，在头文件中声明了
void open_music_file(const char *path_name){
	// 通过fopen函数打开音乐文件
	fp = fopen(path_name, "rb");
	// 判断文件是否为空
	if(fp == NULL){
		printf("music file is NULL");
		exit(1);
	}
	// 把文件指针定位到文件的开头处
	fseek(fp, 0, SEEK_SET);

	// 读取文件，并解析文件头获取有用信息
	wav_header_size = fread(&wav_header, 1, sizeof(wav_header), fp);
	printf("wav文件头结构体大小：	%d 			\n", wav_header_size);
	printf("RIFF标志：		\t 		%c%c%c%c 	\n", wav_header.chunk_id[0], wav_header.chunk_id[1], wav_header.chunk_id[2], wav_header.chunk_id[3]);
	printf("文件大小： 		\t 		%d			\n", wav_header.chunk_size);
	printf("文件格式： 		\t 		%c%c%c%c 	\n", wav_header.format[0], wav_header.format[1], wav_header.format[2], wav_header.format[3]);
	printf("格式块标识：	\t\t\t 	%c%c%c%c	\n", wav_header.sub_chunk1_id[0], wav_header.sub_chunk1_id[1], wav_header.sub_chunk1_id[2], wav_header.sub_chunk1_id[3]);
	printf("格式块长度：	\t\t\t 	%d 			\n", wav_header.sub_chunk1_size);
	printf("编码格式代码: 	\t\t\t 	%d 			\n", wav_header.audio_format);
	printf("声道数:			\t 		%d			\n", wav_header.num_channels);
	printf("采样频率:		\t 		%d			\n", wav_header.sample_rate);
	printf("传输速率： 		\t\t 	%d 			\n", wav_header.byte_rate);
	printf("数据块对齐单位: \t\t\t 	%d			\n", wav_header.block_align);
	printf("采样位数:		\t 		%d			\n", wav_header.bits_per_sample);


}


bool is_set(int result, const char *str)
{
	if(result < 0){
		printf("err: %s 失败! \n", str);
		exit(1);
	}
	return true;
}

