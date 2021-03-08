// Lab2.cpp: 定义应用程序的入口点。
//
/*
 * 设有⼆元函数 f(x, y) = f(x) + f(y)
其中：
f(x) = f(x-1) * x (x >1)
f(x) = 1 (x=1)
f(y) = f(y-1) + f(y-2) (y > 2)
f(y) = 1 (y=1, 2)
请编程建⽴ 3 个并发协作进程或线程，它们分别完成 f(x, y)、f(x)、f(y)


 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


void task_x(int*);
void task_y(int*);

int pipex[2], pipey[2];
pthread_t thrd1, thrd2;

int main(int argc,char*argv[])
{
	int ret;
	int x, y;

	printf("Input x and y:\n");
	scanf(("%d %d"), &x, &y);

	if (pipe(pipex) < 0)
	{
		perror(("Pipex not create!\n"));
		exit(EXIT_FAILURE);
	}

	if(pipe(pipey) < 0)
	{
		perror(("Pipey not create!\n"));
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&thrd1, NULL,(void*)task_x ,(void*)&x);
	if(ret)   //非0出错
	{
		perror("pthread_create:task_x");
		exit(EXIT_FAILURE);
	}

	ret = pthread_create(&thrd2, NULL, (void*)task_y, (void*)&y);
	if (ret)   //非0出错
	{
		perror("pthread_create:task_y");
		exit(EXIT_FAILURE);
	}


	//管道0端读，1端写
	
	int fx = 0, fy = 0;
	int tx = 0, ty = 0;

	printf("x = %d, y = %d\n", x, y);

	while (tx<x || ty<y)
	{
		if(tx < x)
		{
			read(pipex[0], &fx, sizeof(int));
			tx++;
		}
		if(ty < y)
		{
			read(pipey[0], &fy, sizeof(int));
			ty++;
		}
		printf("f(x = %d) = %d, f(y = %d) = %d, f(x,y) = %d\n", tx ,fx, ty ,fy , fx + fy);
	}
	
	close(pipex[0]);
	close(pipey[0]);
	
	exit(EXIT_SUCCESS);
}

void task_x(int* x)
{
	int result_x = 0,  tx = 0;
	while(tx++ < *x)
	{
		if(tx == 1) result_x = 1;
		else result_x = result_x * tx;
		
		write(pipex[1], &result_x, sizeof(int));
	}
	
	close(pipex[1]);
}

void task_y(int *y)
{	
	int i = 0;
	int result_y[2], ty = 0;

	while(ty++ < *y)
	{
		if (ty == 1 || ty == 2) result_y[0] = result_y[1] = 1;
		else
		{
			result_y[(i + 1) % 2] = result_y[i] + result_y[(i + 1) % 2];
			i = (i + 1) % 2;
		}
		write(pipey[1], &result_y[i], sizeof(int));
	}

	close(pipey[1]);
}
