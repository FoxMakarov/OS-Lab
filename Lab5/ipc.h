#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
/*
 *设置账本互斥信号量		accountBook 
 *建立沙发消息队列		sofaMsg
 *建立等候室消息队列		waitingRoomMsg
 *产生新顾客的消息队列    newConsumerMsg
 *产生一个请求结账的消息  askForPayMsg
 *理发师在每次循环检查消息队列
 *
 *barber.c
 *
 *建立三个理发师进程
 *while(1)
 *{
 *	int judge = 0;
 *	if(沙发上有人等待理发)
 *	{
 *		理发；
 *		向请求结账消息队列中发送消息
 *		judge = 1;
 *	}
 *
 *	if(有人想要结账)
 *	{
 *		down(账本);
 *		结账
 *		up(账本);
 *		judge = 1;
 *	}
 *
 *	if(judge == 0)睡觉;
 *	
 *}
 *
 *consumer.c
 *
 *建立一个子进程用来产生顾客
 *
 *主进程
 *while(1)
 *{
 *	if(沙发没坐满)
 *	{
 *		if(等候室里有人)
 *		移到沙发上去
 *	}
 *
 *	if(收到了子进程发来的产生新顾客的消息)
 *	{
 *		if(沙发没坐满)
 *		{
 *			直接坐到沙发上去
 *		}
 *		else
 *		{
 *			if(等待室里没有坐满)
 *				到等待室中去
 *			else
 *			   由于顾客满了所以离开	
 *		}
 *	}
 * }
 *
 *
 *}
 *
 *
 * 
 */


#define BUFSZ 256
#define sofaSize 4
#define waitingRoomSize 13
#define SOFA 1
#define WAITINGROOM 2
#define NEWCONSUMER 3
#define ASKFORPAY 4


/*信号灯控制⽤的共同体*/
typedef union semuns {
	int val;
} Sem_uns;
/* 消 息 结 构 体 */
typedef struct msgbuf {
	long mtype;    //消息类型
	int mid;       //消息文本
} Msg_buf;


/*设置账本互斥信号量		accountBook
 *建立沙发消息队列		sofaMsg
 *建立等候室消息队列		waitingRoomMsg
 *产生新顾客的消息队列    newConsumerMsg
 *结账请求消息队列        askForPayMsg
 **/

key_t accountBook_key;
int accountBook_id;
int semFlg;

key_t sofaMsg_key;
int sofaMsg_id;

key_t waitingRoomMsg_key;
int waitingRoomMsg_id;

key_t newConsumerMsg_key;
int newConsumerMsg_id;

key_t  askForPayMsg_key;
int askForPayMsg_id;

int msgFlg;

struct msqid_ds sofaMsg_info;
struct msqid_ds waitingRoomMsg_info;
struct msqid_ds newConsumerMsg_info;	

int get_ipc_id(char* proc_file, key_t key);
char* set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);