#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	// 接收返回值
	int ret;
	// while 循环是在调用脚本的时候循环获取参数，直到获取到-1时退出，也就是脚本后面可跟多个参数。否则只能跟一个就退出了
	while((ret = getopt(argc,argv,"a:bc::")) != -1)
	{
	    switch(ret)
    	{
    		case 'a':
    		    printf("The argument of -a is: %s \n", optarg);
    		    break;
    		case 'b':
    		    printf("此参数没有额外参数\n");
    		    break;
    		case 'c':
    		    printf("The argument of -c is: %s \n", optarg);
				break;
    		default:
    		    return 0;
    	}
	}
	return 0;
}