// Lab1.cpp: 定义应用程序的入口点。
//

/*
 * 参考以上示例程序中建⽴并发进程的⽅法，编写⼀个多进程并发执⾏程序。⽗进程建⽴两个⼦进程，⾸
先创建的让其执⾏ ls 命令，之后创建执⾏让其执⾏ ps 命令，并控制 ps 命令总在 ls 命令之前执⾏。要求
⽗进程等待两个⼦进程结束。
对于该实验，我们准备了⼀份测试程序，它位于实验⽬录下的 tester.c ⽂件，使⽤⽅法为： ./tester
./<你的程序名> 。请通过测试。
 */

#include "Lab1.h"


int main(int argc, char* argv[]) {

	char* args_ls[] = { "/bin/ls",NULL };
	char* args_ps[] = { "/bin/ps",NULL };

	signal(SIGINT, (sighandler_t)sigcat);
	
	int pid_ls = 0xffff, pid_ps = 0xffff;
	int status_ls = 0, status_ps = 0;

	/*pid_ls = fork();
	pid_ps = fork();*/

	if (pid_ls != 0 && pid_ps != 0) pid_ls = fork();

	if (pid_ls != 0 && pid_ps != 0) pid_ps = fork();
	

	if (pid_ls != 0 && pid_ps != 0)
	{
		if (pid_ps < 0 || pid_ls < 0)
		{
			printf("Create Process failed!\n");
			exit(EXIT_FAILURE);
		}

		waitpid(pid_ps, &status_ps, 0);
		printf("\nMy Child_ps Exit with status = %d\n\n", status_ps);

		kill(pid_ls, SIGINT);

		waitpid(pid_ls, &status_ls, 0);
		printf("\nMy Child_ls Exit with status = %d\n\n", status_ls);

		pause();
		
		exit(EXIT_SUCCESS);
	}

	if(pid_ls == 0)
	{
		pause();

		for (int i = 0; args_ls[i] != NULL; i++)printf(("%s "), args_ls[i]);
		printf("\n");

		status_ls = execve(args_ls[0], args_ls, NULL);		
	}

	if(pid_ps == 0)
	{
		for (int i = 0; args_ps[i] != NULL; i++)printf(("%s "), args_ps[i]);
		printf("\n");

		printf("%d\n", pid_ls);
		
		status_ps = execve(args_ps[0], args_ps, NULL);
	}

	
	
	return EXIT_SUCCESS;
}


