/*
 *单行道上最多有max辆火车一起行驶，建立一个初值为max的用于锁的信号灯，管程中能够同时活跃的进程数目最大为max
 *设立一个condition条件变量用于某辆火车进入等待或是唤醒另一个方向的火车
 *
 *为了避免饥饿设置某一个方向最多能够通过的火车的数目就是max
 *
 *管程OneWay里有三个重要的函数，表示火车的三种状态
 *
 *arrive:
 *当产生火车的时候会调用这个函数，用来表示某辆火车到了起始站
 *对于这辆火车是直接上路还是进入等待也是在这个函数中进行判断
 *
 *对于需要进入等待的火车有两种情况：
 *方向与当前的方向不一样,并且不是初始情况
 *方向与当前方向一致并且这个方向已经走上的火车的数目已经达到了最大值
 *
 * cross:火车在路上走
 *
 * leave:
 * 火车到达后，现在单行道上的车的数目--
 *
 * 当单行道数目为0时，考虑唤醒另一边的车
 * 优先唤醒另一个方向的车，如果另一个方向没车再唤醒自己这个方向的车，如果两边都没有车，回归最初状态
 *
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <algorithm>
 /*信号灯控制⽤的共同体*/
typedef union semuns { int val; } Sem_uns;

//单程道管程中使用的信号量
class Sema {
public:
	Sema(int id);
	~Sema() = default;
	int down(); //信号量加 1
	int up(); //信号量减 1
private:
	int sem_id; //信号量标识符
};

//单行道管程中使用的锁
class Lock {
public:
	Lock(Sema* _sema);
	~Lock() = default;
	void close_lock();
	void open_lock();
private:
	Sema* sema; //锁使⽤的信号量
};

class Condition
{
public:
	Condition(Sema* _sema0, Sema* _sema1);

	~Condition() = default;

	void wait(Lock* lock, int direct);

	void signal(int direct);  //唤醒另一方向的火车

private:
	//两个方向信号量
	Sema* sema0;  //由南到北
	Sema* sema1;  //由北到南
};

class OneWay
{
public:
	OneWay(int maxNowTrain);

	~OneWay() = default;

	//火车到达车站在等待排队
	void arrive(int direct);

	//火车在路上行驶
	void cross(int direct);

	//火车抵达了对面的车站离开了
	void leave(int direct);

private:
	int* southWait;  //在南边火车站等待的火车的数目
	int* northWait;  //在北边火车站等待的火车的数目
	int* maxCurInWay; //允许的最大单行道上的车的数目
	int* currentDirect;  //当前单行道上的火车方向
	int* currentNumber;   //当前单行道上的火车数量
	int* currentDirectPassed;  //当前方向上火车经过的数量
	int* allPassed;  //单行道上从初始状态开始通过的车的总数
	
	Condition* condition;  //通过单行道的条件变量

	Lock* curLock; //单行道管程锁


	//建立或获取 ipc 信号量的一组函数的原型说明
	int get_ipc_id(char* proc_file, key_t key);

	int set_sem(key_t sem_key, int sem_val, int sem_flg);

	char* set_shm(key_t shm_key, int shm_num, int shm_flg);
};

inline Sema::Sema(int id)
{
	sem_id = id;
}

inline int Sema::down()
{
	struct sembuf buf;
	buf.sem_op = -1;
	buf.sem_num = 0;
	buf.sem_flg = SEM_UNDO;
	if ((semop(sem_id, &buf, 1)) < 0) {
		perror("down error ");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

inline int Sema::up()
{
	Sem_uns arg;
	struct sembuf buf;
	buf.sem_op = 1;
	buf.sem_num = 0;
	buf.sem_flg = SEM_UNDO;
	if ((semop(sem_id, &buf, 1)) < 0) {
		perror("up error ");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}

inline Lock::Lock(Sema* _sema)
{
	sema = _sema;
}

inline void Lock::close_lock()
{
	sema->down();
}

inline void Lock::open_lock()
{
	sema->up();
}

inline Condition::Condition(Sema* _sema0, Sema* _sema1)
{
	sema0 = _sema0;
	sema1 = _sema1;
}

inline void Condition::wait(Lock* lock, int direct)
{
	if(direct == 0) //由南向北
	{
		std::cout << getpid() << " train wait from south to north.\n";
		lock->open_lock();
		sema0->down();
		lock->close_lock();
	}
	else           //由北向南
	{
		std::cout << getpid() << " train wait from north to south.\n";
		lock->open_lock();
		sema1->down();
		lock->close_lock();
	}
}

inline void Condition::signal(int direct)  //direct:0表示唤醒从南到北，direct:1表示唤醒从北到南
{
	if (direct == 0) sema0->up();
	else sema1->up();
}

/*
 * get_ipc_id() 从/proc/sysvipc/⽂件系统中获取 IPC 的 id 号
 * pfile: 对应/proc/sysvipc/⽬录中的 IPC ⽂件分别为
 * msg-消息队列,sem-信号量,shm-共享内存
 * key: 对应要获取的 IPC 的 id 号的键值
*/
inline int OneWay::get_ipc_id(char* proc_file, key_t key)
{
#define BUFSZ 256
	FILE* pf;
	int i, j;
	char line[BUFSZ], colum[BUFSZ];
	if ((pf = fopen(proc_file, "r")) == NULL) {
		perror("Proc file not open");
		exit(EXIT_FAILURE);
	}
	fgets(line, BUFSZ, pf);
	while (!feof(pf)) {
		i = j = 0;
		fgets(line, BUFSZ, pf);
		while (line[i] == ' ')
			i++;
		while (line[i] != ' ')
			colum[j++] = line[i++];
		colum[j] = '\0';
		if (atoi(colum) != key)
			continue;
		j = 0;
		while (line[i] == ' ')
			i++;
		while (line[i] != ' ')
			colum[j++] = line[i++];
		colum[j] = '\0';
		i = atoi(colum);
		fclose(pf);
		return i;
	}
	fclose(pf);
	return -1;
}

/*
 * set_sem 函数建⽴⼀个具有 n 个信号灯的信号量
 * 如果建⽴成功，返回 ⼀个信号量的标识符 sem_id
 * 输⼊参数：
 * sem_key 信号量的键值
 * sem_val 信号量中信号灯的个数
 * sem_flag 信号量的存取权限
*/
inline int OneWay::set_sem(key_t sem_key, int sem_val, int sem_flg)
{
	int sem_id;
	Sem_uns sem_arg;
	//测试由 sem_key 标识的信号量是否已经建⽴
	if ((sem_id = get_ipc_id("/proc/sysvipc/sem", sem_key)) < 0) {
		//semget 新建⼀个信号灯,其标号返回到sem_id
		if ((sem_id = semget(sem_key, 1, sem_flg)) < 0) {
			perror("semaphore create error");
			exit(EXIT_FAILURE);
		}
	}
	//设置信号量的初值
	sem_arg.val = sem_val;
	if (semctl(sem_id, 0, SETVAL, sem_arg) < 0) {
		perror("semaphore set error");
		exit(EXIT_FAILURE);
	}
	return sem_id;
}

/*
 * set_shm 函数建⽴⼀个具有 n 个字节 的共享内存区
 * 如果建⽴成功，返回 ⼀个指向该内存区⾸地址的指针 shm_buf
 * 输⼊参数：
 * shm_key 共享内存的键值
 * shm_val 共享内存字节的⻓度
 * shm_flag 共享内存的存取权限
*/
inline char* OneWay::set_shm(key_t shm_key, int shm_num, int shm_flg)
{
	int i, shm_id;
	char* shm_buf;
	//测试由 shm_key 标识的共享内存区是否已经建⽴
	if ((shm_id = get_ipc_id("/proc/sysvipc/shm", shm_key)) < 0) {
		//shmget 新建 ⼀个⻓度为 shm_num 字节的共享内存
		if ((shm_id = shmget(shm_key, shm_num, shm_flg)) < 0) {
			perror("shareMemory set error");
			exit(EXIT_FAILURE);
		}
		//shmat 将由 shm_id 标识的共享内存附加给指针 shm_buf
		if ((shm_buf = (char*)shmat(shm_id, 0, 0)) < (char*)0) {
			perror("get shareMemory error");
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < shm_num; i++)
			shm_buf[i] = 0; //初始为 0
	}
	//共享内存区已经建⽴,将由 shm_id 标识的共享内存附加给指针 shm_buf
	if ((shm_buf = (char*)shmat(shm_id, 0, 0)) < (char*)0) {
		perror("get shareMemory error");
		exit(EXIT_FAILURE);
	}
	return shm_buf;
}

inline OneWay::OneWay(int maxNowTrain)
{
	int ipc_flg = IPC_CREAT | 0644;
	int ipc_key = 200;
	southWait = (int*)set_shm(ipc_key++, 1, ipc_flg);
	northWait = (int*)set_shm(ipc_key++, 1, ipc_flg);
	currentDirect = (int*)set_shm(ipc_key++, 1, ipc_flg);
	currentNumber = (int*)set_shm(ipc_key++, 1, ipc_flg);
	currentDirectPassed = (int*)set_shm(ipc_key++, 1, ipc_flg);
	maxCurInWay = (int*)set_shm(ipc_key++, 1, ipc_flg);
	allPassed = (int*)set_shm(ipc_key++, 1, ipc_flg);;

	*maxCurInWay = maxNowTrain;
	*southWait = 0;
	*northWait = 0;
	*currentDirect = 0;
	*currentNumber = 0;
	*currentDirectPassed = 0;
	*allPassed = 0;

	int sema0_id = set_sem(ipc_key++, 0, ipc_flg);
	int sema1_id = set_sem(ipc_key++, 0, ipc_flg);
	int semaCurLock_id = set_sem(ipc_key++, maxNowTrain, ipc_flg);
	
	
	Sema* _sema0 = new Sema(sema0_id);
	Sema* _sema1 = new Sema(sema1_id);
	Sema* _semaCurLock = new Sema(semaCurLock_id);
	

	condition = new Condition(_sema0, _sema1);

	curLock = new Lock(_semaCurLock);
	
}


inline void OneWay::arrive(int direct)
{
	curLock->close_lock();

	if (direct == 0)
		std::cout << "Train " << getpid() << " arrive at south station.\n";
	else
		std::cout << "Train " << getpid() << " arrive at north station.\n";

	
	if((direct != *currentDirect && *allPassed != 0) || (direct == *currentDirect && *currentDirectPassed >= *maxCurInWay))
	{
		if (direct == 0)*southWait = *southWait + 1;
		else *northWait = *northWait + 1;

		condition->wait(curLock, direct);

		if(*currentDirect != direct) //代表这辆车是这个方向的第一个
		{
			*currentDirectPassed = 0;
			*currentDirect = direct;
		}
		
	}
	if(direct == 0)
	{
		*southWait = *southWait - 1;
	}
	else
	{
		*northWait = *northWait -1;
	}
	
	*currentDirect = direct;
	*currentNumber = *currentNumber + 1;
	*currentDirectPassed = *currentDirectPassed + 1;
	*allPassed = *allPassed + 1;
	curLock->open_lock();
}

inline void OneWay::cross(int direct)
{
	curLock->close_lock();
	std::cout << "Train " << getpid() << (direct == 0 ? " is coming to north" : " is coming to south") << std::endl;
	sleep(6);
	curLock->open_lock();
}

inline void OneWay::leave(int direct)
{
	curLock->close_lock();
	*currentNumber = *currentNumber - 1;
	std::cout << "Train " << getpid() << " left." << std::endl;

	if(*currentNumber == 0)  //当前单行道上无车，优先唤醒另一个方向上的车，如果没有再唤醒自己者方向上的车
	{
		if(direct == 0)
		{
			if (*northWait > 0)
				condition->signal(1);
			else if (*southWait > 0)
				condition->signal(0);
			else  //回归初始状态
			{
				*allPassed = 0;
				*currentDirectPassed = 0;  
			}
		}
		else
		{
			if (*southWait > 0)
				condition->signal(0);
			else if (*northWait > 0)
				condition->signal(1);
			else  //回归初始状态
			{
				*allPassed = 0;
				*currentDirectPassed = 0;
			}
		}
	}
	curLock->open_lock();
}


