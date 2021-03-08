
#include "ipc.h"
int main(int argc, char* argv[]) {
	int rate = 3;

	int producer = atoi(argv[1]);

	cget_key = 200;
	cget_num = 1;
	shm_flg = IPC_CREAT | 0644;

	cget_ptr = (int*)set_shm(cget_key, cget_num, shm_flg);//用共享内存中的值表明当前是第几个抽烟者

	producer_key = 201;//生产者同步信号灯键值
	A_key = 202;
	B_key = 203;
	C_key = 204;

	sem_flg = IPC_CREAT | 0644;

	sem_val = 1;//生产者最初可以生产，设为1
	producer_id = set_sem(producer_key, sem_val, sem_flg);

	sem_val = 0;//消费者最初都没有办法消费，设为0
	A_id = set_sem(A_key, sem_val, sem_flg);
	B_id = set_sem(B_key, sem_val, sem_flg);
	C_id = set_sem(C_key, sem_val, sem_flg);

	//在三种情况下轮流
	while (1) {
		down(producer_id);
		sleep(rate);
		
		if(*cget_ptr == 0)
		{
			printf("Producer  %d produce tobacco and glue for consumerA.\n", producer);
			up(A_id);
		}
		if (*cget_ptr == 1)
		{
			printf("Producer  %d produce tobacco and paper for consumerB.\n", producer);
			up(B_id);
		}
		if (*cget_ptr == 2)
		{
			printf("Producer  %d produce paper and glue for consumerC.\n", producer);
			up(C_id);
		}
		
		*cget_ptr = (*cget_ptr + 1) % 3;
		
	}

	return EXIT_SUCCESS;
}