// Lab1.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> // 进程⾃定义的键盘中断信号处理函数
typedef void (*sighandler_t) (int);
void sigcat() {
	printf("%d Process continue\n", getpid());
}
// TODO: 在此处引用程序需要的其他标头。
