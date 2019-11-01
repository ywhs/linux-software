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
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <alsa/asoundlib.h>
#include "const.h"


int main(int argc, char *argv [])
{

	int ret, rate_arg, format_arg;
	bool flag = true;


#if 0
	// 判断命令参数中是携带音乐文件
	if(argc != 2){
		printf("err: not found music file \n");
		exit(1);
	}
	// 打开音乐文件
	open_music_file(argv[1]);

#endif

	while((ret = getopt(argc,argv,"m:f:r:")) != -1){
		flag = false;
		switch(ret){
			case 'm':
				printf("open music file \n");
				open_music_file(optarg);
				break;
			case 'f':
				format_arg = atoi(optarg);
				printf("format_arg value is :%d \n", format_arg);
				if(format_arg == 16){
					//period_size = 8192 * 2;
					pcm_format = SND_PCM_FORMAT_S16_LE;
				}else if(format_arg == 32){
					//period_size = 8192 * 4;
					pcm_format = SND_PCM_FORMAT_S32_LE;
				}else if(format_arg == 24){
					pcm_format = SND_PCM_FORMAT_S24_LE;
				}else{
					pcm_format = SND_PCM_FORMAT_S8;
				}
				break;
			case 'r':
				rate_arg = atoi(optarg);
				printf("rate_arg value is :%d \n", rate_arg);
				if(rate_arg == 44){
					rate = 44100;
				}else if(rate_arg == 88){
					rate = 88200;
				}else{
					rate = rate_arg;
				}
				break;
			default:
				printf("use default value! \n");
				open_music_file(optarg);
				pcm_format = SND_PCM_FORMAT_S16_LE;
				rate = 44100;
				break;
		}
	}

	if(flag){
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		printf("Either 1'st, 2'nd, 3'th or all parameters were missing \n");
		printf("\n");
		printf("1'st parameter can be one of these: -m(music_filename) \n");
		printf("2'nd parameter can be one of these: -f(format,8bit or 16bit or 32bit) \n");
		printf("3'th parameter can be one of these: -r(rate,44 or 88) \n");
		printf("For example: alsa -m 1.wav -f 16 -r 44 \n");
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		exit(1);
	}
	

	
	// 在堆栈上分配snd_pcm_hw_params_t结构体的空间，参数是配置pcm硬件的指针,返回0成功
	debug_msg(snd_pcm_hw_params_malloc(&hw_params), "分配snd_pcm_hw_params_t结构体");

	

	// 打开PCM设备 返回0 则成功，其他失败
	// 函数的最后一个参数是配置模式，如果设置为0,则使用标准模式
	// 其他值位SND_PCM_NONBLOCL和SND_PCM_ASYNC 如果使用NONBLOCL 则读/写访问, 如果是后一个就会发出SIGIO（没太懂什么意思）
	pcm_name = strdup("hw:0,0");
	debug_msg(snd_pcm_open(&pcm_handle, pcm_name, stream, 0), "打开PCM设备");



	// 在我们将PCM数据写入声卡之前，我们必须指定访问类型，样本长度，采样率，通道数，周期数和周期大小。
	// 首先，我们使用声卡的完整配置空间之前初始化hwparams结构
	debug_msg(snd_pcm_hw_params_any(pcm_handle, hw_params), "配置空间初始化");




	
	// 设置交错模式（访问模式）
	// 常用的有 SND_PCM_ACCESS_RW_INTERLEAVED（交错模式） 和 SND_PCM_ACCESS_RW_NONINTERLEAVED （非交错模式） 
	// 参考：https://blog.51cto.com/yiluohuanghun/868048
	debug_msg(snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED), "设置交错模式（访问模式）");




	

	// 设置采样位数 根据音乐文件返回的采样位数设置
#if 0
	if(wav_header.bits_per_sample == 8){
		pcm_format = SND_PCM_FORMAT_S8;
	}else if(wav_header.bits_per_sample == 16){
		pcm_format = SND_PCM_FORMAT_S16_LE;
	}

#endif
	debug_msg(snd_pcm_hw_params_set_format(pcm_handle, hw_params, pcm_format), "设置样本长度(位数)");




	

	// 设置采样率为44.1KHz dir的范围(-1,0,1) 88.2
	//rate = wav_header.sample_rate;
	debug_msg(snd_pcm_hw_params_set_rate_near(pcm_handle, hw_params, &rate, 0), "设置采样率");
	printf("rate value is : %d \n", rate);

	

	// 设置通道数
	debug_msg(snd_pcm_hw_params_set_channels(pcm_handle, hw_params, wav_header.num_channels), "设置通道数");



	// 设置periods: 一个缓冲区所包含的周期数.
	debug_msg(snd_pcm_hw_params_set_periods(pcm_handle, hw_params, periods, 0), "设置周期数");

	


	// 设置缓冲区 buffer_size = period_size * periods
	buffer_size = period_size * periods;
	if(format_arg == 16){
		/*
			snd_pcm_hw_params_set_buffer_size() 最后一个参数是要设置的缓冲区有多大，单位是帧(frames_size),
			一个缓冲区的大小可以这么算，我上面设定了周期为2，周期大小我们预先自己设定，那么要设置的缓存大小就是 周期大小 * 周期数 就是缓冲区大小了。
			
			那么为什么要再除以4呢，因为参数需要传进的是以帧为单位,所以要先把字节大小先转换为帧
			
			一个帧的大小 = 采样位数（长度） * 通道数，例如：我上面的采样位数位16bit，通道数为2，那么一个帧的大小就是32bit/8=4个字节
			所以除以4以后就算出来缓冲区的大小了,其实缓冲区的大小还是 period_size * periods 个字节，只不过把这些数据变成了另一种单位传进去而已
		*/
		debug_msg(snd_pcm_hw_params_set_buffer_size(pcm_handle, hw_params, buffer_size>>2), "设置缓冲区");
	}else if(format_arg == 32){
		/*
			如果按照上面的理解的话，那么当位数为32时，就需要除以8了
		*/
		debug_msg(snd_pcm_hw_params_set_buffer_size(pcm_handle, hw_params, buffer_size>>3), "设置缓冲区");
	}
	


	// 设置的硬件配置参数，加载，并且会自动调用snd_pcm_prepare()将stream状态置为SND_PCM_STATE_PREPARED
	debug_msg(snd_pcm_hw_params(pcm_handle, hw_params), "设置的硬件配置参数");


#if 0
	/*
		snd_pcm_hw_params_get_period_size返回的是以帧为单位的数，如果想换算成字节数，如果是16位乘以4，32位乘以8就是字节数
	 	正好是一个周期大小的字节数
	*/
	debug_msg(snd_pcm_hw_params_get_period_size(hw_params, &frames, 0), "获取返回的近似周期大小");
	printf("frames value is : %ld \n", frames * 4);



	// snd_pcm_hw_params_get_buffer_size函数也是返回的以帧为单位的数，想换算成字节数，也需要根据不同bit乘以不同的数字
	debug_msg(snd_pcm_hw_params_get_buffer_size(hw_params, &period_size), "获取缓冲区大小");
	printf("buffer_size value is : %ld \n", period_size * 4);

	exit(1);
#endif

	/*
		这里为buffer_size分配的空间为一个周期的大小，这里的单位就变成了字节，分配的是内存空间，所以就是原本的字节大小
		ALSA中的字节与帧的概念一定要搞清楚，不能混淆，要不太麻烦了，也不知道我这样理解是不是对的
		缓存大小这还需要再理解理解，现在还是懵，反正这个数值设下去，在虚拟机中的linux能播放音乐，但是在另一个平台播放音乐就断断续续的
		所以还要再研究研究。
	*/
	buff = (unsigned char *)malloc(period_size);

#if 0
	buff1 = (unsigned char *)malloc(period_size * 2);
	memset(buff1, 0x00, period_size * 2);
#endif

	// snd_pcm_writei()函数第三个是帧单位，所以frames要通过除以4变成帧单位,除以4是以采样长度16bit来进行除的
	// 意思是 这些个（period_size）大小缓存中有多少个frames（左右声道 * 一个采样位数）
	frames = period_size >> 2;

	
	// feof函数检测文件结束符，结束：非0, 没结束：0 !feof(fp)
	while(1){
		// 读取文件数据放到缓存中
		ret = fread(buff, 1, period_size, fp);
		
		if(ret == 0){
			printf("end of music file input! \n");
			exit(1);
		}
		
		if(ret < 0){
			debug_msg(-1, "read pcm from file \n");
		}

#if 0
		for(int i =0; i < frames; i++){
			buff1[i*4] = buff[i*2];
			buff1[i*4+1] = buff[i*2+1];
		}
#endif
		// 向PCM设备写入数据
		while((ret = snd_pcm_writei(pcm_handle, buff, frames)) < 0){
			if (ret == -EPIPE){
                  /* EPIPE means underrun */
                  printf("underrun occurred -32 \n");
				  //完成硬件参数设置，使设备准备好
                  snd_pcm_prepare(pcm_handle);
            }else if(ret < 0){
				printf("ret value is : %d \n", ret);
				debug_msg(-1, "write to audio interface failed");
			}
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
		printf("music file is NULL \n");
		fclose(fp);
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
	printf("采样位数(长度):		\t 		%d			\n", wav_header.bits_per_sample);


}


bool debug_msg(int result, const char *str)
{
	if(result < 0){
		printf("err: %s 失败! \n", str);
		exit(1);
	}
	return true;
}

