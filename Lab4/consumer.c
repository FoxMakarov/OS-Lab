
#include "ipc.h"
int main(int argc, char* argv[]) {
	int rate = 3;

	int consumer = atoi(argv[1]);

	producer_key = 201;
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

	if (consumer == 0) printf("ConsumerA has paper.\n");
	if (consumer == 1) printf("ConsumerB has glue.\n");
	if (consumer == 2) printf("ConsumerC has tobacco.\n");
	
	while (1) {
		
		if(consumer == 0)
		{
			down(A_id);
			sleep(rate);
			printf("ConsumerA has tobacco and glue now.\n");
		}

		if (consumer == 1)
		{
			down(B_id);
			sleep(rate);
			printf("ConsumerB has tobacco and paper now.\n");
		}

		if (consumer == 2)
		{
			down(C_id);
			sleep(rate);
			printf("ConsumerC has paper and glue now.\n");
		}

		up(producer_id);
		
	}
	return EXIT_SUCCESS;
}