#include "dp.h"
#include <time.h>


int main()
{	
	int maxnum;
	printf("Please input max number of trains in way:\n");
	scanf("%d", &maxnum);

	OneWay oneway(maxnum);

	int pid_id[100]; //最多100趟车

	for(int i = 0;i<100;i++)
	{
		sleep(2);
		pid_id[i] = fork();
		if(pid_id[i] == 0)
		{
			srand((int)time(NULL));
			int direct = rand() % 2;
			oneway.arrive(direct);
			oneway.cross(direct);
			oneway.leave(direct);
			exit(EXIT_SUCCESS);
		}
	}
}
