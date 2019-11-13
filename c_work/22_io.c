#include <stdio.h>

int main(){

	// 定义一个file类型的变量
	FILE *fp = NULL;
	
	// 打开一个文件, w+允许读写文件，并判断此文件是否存在，如果不存在则创建这个文件
	fp = fopen("test.txt", "w+");
	// 向这个文件中写入内容
	fprintf(fp, "This is testing for fprintf...s123\n");
	fputs("This is testing for fputs...s123\n", fp);
	// 关闭文件
	fclose(fp);
	
	readFile();

	return 0;

}

readFile(){

	// 定义一个文件类型变量
	FILE *fp = NULL;
	// 定义缓冲区
	char buff[255];

	// 打开文件, 读取模式
	fp = fopen("test.txt", "r");
	// 开始读取文件内容，fscanf函数遇到第一个空格就不再读取了
	fscanf(fp, "%s", buff);
	printf("1: %s\n", buff);

	// 使用fgets获取余下的内容
	fgets(buff, 255, (FILE*)fp);
	printf("2: %s", buff);

	// 获取第二行中的内容
	fgets(buff, 255, (FILE*)fp);
	printf("3: %s", buff);

}
