#include "apue.h"


int main()
{
	// 获取线程的id，方法名是 pthread_self() 返回的类型是一个 无符号的整型，pthread_t类型,在 /usr/include/bits/pthreadtypes.h中有定义
	// 定义pthread_t的变量,线程类
	pthread_t tid;
	// 定义pid_t类型变量, 进程类
	pid_t pid;

	// 获取线程id
	tid = pthread_self();
	
	// 获取进程id
	pid = getpid();

	printf("线程id tid %u \n", tid);
	printf("进程id pid %u \n", pid);
	return 0;
}
