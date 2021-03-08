#include <signal.h>
#include <time.h>
#include "ipc.h"

int pid_id;

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

	pid_id = fork();

	if(pid_id == 0)
	{
		int i = 0;
		struct msgbuf newConsumer_buf;
		newConsumer_buf.mtype = NEWCONSUMER;

		while(1)
		{
			newConsumer_buf.mid = i++;
			
			msgsnd(newConsumerMsg_id, &newConsumer_buf, sizeof(newConsumer_buf), 0);

			sleep(1);
		}
	}

	while(1)
	{
		msgctl(sofaMsg_id, IPC_STAT, &sofaMsg_info);
		
		if (sofaMsg_info.msg_qnum < sofaSize)
		{
			struct msgbuf waitingRoom_buf;
			if (msgrcv(waitingRoomMsg_id, &waitingRoom_buf, sizeof(waitingRoom_buf), WAITINGROOM, IPC_NOWAIT) >= 0)
			{
				printf("%d consumer sits on the sofa.\n", waitingRoom_buf.mid);

				waitingRoom_buf.mtype = SOFA;
				msgsnd(sofaMsg_id, &waitingRoom_buf, sizeof(waitingRoom_buf), 0);
			}

		}

		struct msgbuf newConsumer_buf;
		if(msgrcv(newConsumerMsg_id,&newConsumer_buf,sizeof(newConsumer_buf),NEWCONSUMER,IPC_NOWAIT) >= 0)
		{
			msgctl(sofaMsg_id, IPC_STAT, &sofaMsg_info);
			msgctl(waitingRoomMsg_id, IPC_STAT, &waitingRoomMsg_info);

			if (sofaMsg_info.msg_qnum < sofaSize)
			{
				printf("%d consumer sits on the sofa.\n", newConsumer_buf.mid);

				newConsumer_buf.mtype = SOFA;
				msgsnd(sofaMsg_id, &newConsumer_buf, sizeof(newConsumer_buf), 0);
			}
			else if(waitingRoomMsg_info.msg_qnum >= waitingRoomSize)
			{
				printf("%d consumer leave because it is full.\n",newConsumer_buf.mid);
			}
			else
			{
				printf("%d consumer enters the waiting-room.\n", newConsumer_buf.mid);

				newConsumer_buf.mtype = WAITINGROOM;
				msgsnd(waitingRoomMsg_id, &newConsumer_buf, sizeof(newConsumer_buf), 0);
			}
		}		
	}
}