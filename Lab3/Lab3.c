// Lab3.cpp: 定义应用程序的入口点。
//

#include <stdbool.h>
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

int pid_parent;
int pid_child;
typedef void (*sighandler_t)(int);
void sigcat()
{
	if (getpid() == pid_parent)
	{
		kill(pid_child, SIGINT);
	}
}



char commond[200];
char* argv[100];
char* str;
bool has_file_input = false;
bool has_file_output = false;
char* filename_input;
char* filename_output;
int i = 0;
int fd;
int beforeInput[2] = { false };
int pipe0[2], pipe1[2];

void execute(bool);

int main()
{
	pid_parent = getpid();
	signal(SIGINT, (sighandler_t)sigcat);
	while (gets(commond))
	{
		str = strtok(commond, " ");
		i = 0;
		while (str != NULL)
		{
			if (strcmp(str, ">") == 0)
			{
				str = strtok(NULL, " ");
				filename_output = str;
				has_file_output = true;
			}
			else if (strcmp(str, "<") == 0)
			{
				str = strtok(NULL, " ");
				filename_input = str;
				has_file_input = true;
			}
			else if (strcmp(str, "|") == 0)
			{
				execute(true);
				i = 0;
			}
			else argv[i++] = str;
			str = strtok(NULL, " ");
		}

		if (i != 0)
		{
			execute(false);
		}


	}
}

void execute(bool output_to_pipe)
{
	//一个指令不会同时有文件输入又有管道输入
	//不会同时有文件输出又有管道输出

	int judge = 0;
	if (output_to_pipe)judge += 3;
	if (!(beforeInput[0] || beforeInput[1]))judge += 1;
	if (beforeInput[0])judge += 2;
	if (beforeInput[1])judge += 3;

	//fprintf(stderr, "%d %d\n", judge, (int)output_to_pipe);

	/*
	* judge == 1  没有上一步的管道输入，没有下一步的管道输出
	* judge == 2  管道0读入 ,没有下一步的管道输出
	* judge == 3  管道1读入 ,没有下一步的管道输出
	* judge == 4  没有上一步的管道输入,用管道0做输出
	* judge == 5  管道0读入，管道1输出
	* judge == 6  管道1读入，管道0输出
	*
	*/

	if (judge == 2) beforeInput[0] = false;
	if (judge == 3) beforeInput[1] = false;
	if (judge == 4)
	{
		pipe(pipe0);
		beforeInput[0] = true;
	}
	if (judge == 5)
	{
		beforeInput[0] = false;
		pipe(pipe1);
		beforeInput[1] = true;
	}
	if (judge == 6)
	{
		beforeInput[1] = false;
		pipe(pipe0);
		beforeInput[0] = true;
	}

	if ((pid_child = fork()) < 0)
	{
		perror("Process not create");
		exit(EXIT_FAILURE);
	}
	else if (pid_child == 0)
	{
		argv[i] = NULL;
		if (has_file_input)
		{
			fd = open(filename_input, O_RDONLY | O_CREAT);
			close(0);
			dup(fd);
		}
		if (has_file_output)
		{
			fd = open(filename_output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			close(1);
			dup(fd);
		}
		if (judge == 2) //管道0读入 ,没有下一步的管道输出
		{
			close(0);
			close(pipe0[1]);
			dup(pipe0[0]);

		}
		if (judge == 3) //管道1读入, 没有下一步的管道输出
		{
			close(0);
			close(pipe1[1]);
			dup(pipe1[0]);
		}
		if (judge == 4) //没有上一步的管道输入, 用管道0做输出
		{
			close(1);
			close(pipe0[0]);
			dup(pipe0[1]);
		}
		if (judge == 5) //管道0读入，管道1输出
		{
			close(0);
			close(pipe0[1]);
			dup(pipe0[0]);

			close(1);
			close(pipe1[0]);
			dup(pipe1[1]);
		}

		if (judge == 6) //管道1读入，管道0输出
		{
			close(0);
			close(pipe1[1]);
			dup(pipe1[0]);

			close(1);
			close(pipe0[0]);
			dup(pipe0[1]);
		}

		execvp(argv[0], argv);
	}
	else
	{
		wait(NULL);
		if (judge == 2 || judge == 5)close(pipe0[0]);
		if (judge == 3 || judge == 6)close(pipe1[0]);
		if (judge == 4 || judge == 6)close(pipe0[1]);
		if (judge == 5)close(pipe1[1]);

		if (has_file_input)has_file_input = false;
		if (has_file_output)has_file_output = false;
	}


}