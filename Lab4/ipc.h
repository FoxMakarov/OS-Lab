/*
 *A:
 *1个生产者信号量
 *1个共享内存
 *3个抽烟者信号量
 *
 *producer.c
 *while(1)
 *{
 *	down(生产者信号量)
 *	从共享内存中得出该生产i抽烟者的材料
 *	共享内存里的值变为i+1
 *	up(i号抽烟者信号量)
 *}
 *
 *consumer.c
 *while(1)
 *{
 *	down(i号抽烟者信号量)
 *	抽烟
 *	up(生产者信号量)
 *}
 *
 *B:
 *一个请求材料消息队列
 *一个响应材料消息队列
 *
 *producer:
 *两个生产者进程
 *while(1)
 *{
 *	以阻塞的方式接收请求材料队列的第一个请求
 *	按照收到消息给出响应的是第几号抽烟者即消息中的mtype是第几号抽烟者
 *	向响应材料中发送消息
 *}
 *
 *consumer:
 *三个抽烟者进程
 *while(1)
 *{
 *	以阻塞的方式从相应材料队列中收到第一个属于自己的材料
 *	抽烟
 *	向请求材料队列中发送自己的消息
 *}
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#define BUFSZ 256
//建⽴或获取 ipc 的⼀组函数的原型说明
int get_ipc_id(char* proc_file, key_t key);
char* set_shm(key_t shm_key, int shm_num, int shm_flag);
int set_msq(key_t msq_key, int msq_flag);
int set_sem(key_t sem_key, int sem_val, int sem_flag);
int down(int sem_id);
int up(int sem_id);
/*信号灯控制⽤的共同体*/
typedef union semuns {
	int val;
} Sem_uns;
/* 消 息 结 构 体 */
typedef struct msgbuf {
	long mtype;
	char mtext[1];
} Msg_buf;


key_t producer_key;
key_t A_key;
key_t B_key;
key_t C_key;

int producer_id;
int A_id;
int B_id;
int C_id;


//⽣产消费者共享缓冲区即其有关的变量
key_t buff_key;
int buff_num;
char* buff_ptr;
//⽣产者放产品位置的共享指针
key_t pput_key;
int pput_num;
int* pput_ptr;
//消费者取产品位置的共享指针
key_t cget_key;
int cget_num;
int* cget_ptr;
//⽣产者有关的信号量
key_t prod_key;
key_t pmtx_key;
int prod_sem;
int pmtx_sem;
//消费者有关的信号量
key_t cons_key;
key_t cmtx_key;
int cons_sem;
int cmtx_sem;
int sem_val;
int sem_flg;
int shm_flg;
