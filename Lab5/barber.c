#include <signal.h>
#include <time.h>

#include "ipc.h"

int pid_id[3];

int main()
{

	accountBook_key = 201;
	semFlg = IPC_CREAT | 0644;
	accountBook_id = set_sem(accountBook_key, 1, semFlg);

	msgFlg = IPC_CREAT | 0644;
	sofaMsg_key = 202;
	sofaMsg_id = set_msq(sofaMsg_key, msgFlg);

	waitingRoomMsg_key = 203;
	waitingRoomMsg_id = set_msq(waitingRoomMsg_key, msgFlg);

	newConsumerMsg_key = 204;
	newConsumerMsg_id = set_msq(newConsumerMsg_key, msgFlg);

	askForPayMsg_key = 205;
	askForPayMsg_id = set_msq(askForPayMsg_key, msgFlg);

	int i = 0;

	for (i = 0; i < 3; i++)
	{
		pid_id[i] = fork();

		if (pid_id[i] == 0)
		{
			struct msgbuf sofa_buf;
			struct msgbuf pay_buf;

			int notSleep = 0;
			int beforeState = 1;
			
			while (1)
			{	
				if(msgrcv(sofaMsg_id, &sofa_buf, sizeof(sofa_buf), SOFA, IPC_NOWAIT) >=0 )
				{
					printf("%d barber is giving %d customer a hairdressing.\n", getpid(), sofa_buf.mid);

					sleep(10);

					sofa_buf.mtype = ASKFORPAY;

					msgsnd(askForPayMsg_id, &sofa_buf, sizeof(sofa_buf), 0);

					notSleep = 1;
				}

				if(msgrcv(askForPayMsg_id,&pay_buf,sizeof(pay_buf),ASKFORPAY,IPC_NOWAIT) >= 0)
				{
					down(accountBook_id);

					printf("%d barber is checking out %d customer.\n", getpid(), sofa_buf.mid);
					sleep(3);

					up(accountBook_id);

					notSleep = 1;
				}
				
				if(notSleep == 0 && beforeState == 1)
				{
					printf("Barber %d is sleeping.\n",getpid());
					beforeState = 0;
				}

				if (notSleep == 1) beforeState = 1;
				
			}
		}
	}
	pause();
	
}
