// Lab3.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int pid_bash;
typedef void (*sighandler_t)(int);
void sigcat()
{
	if (getppid() != pid_bash)exit(EXIT_SUCCESS);
}

// TODO: 在此处引用程序需要的其他标头。
