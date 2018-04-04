#ifndef __COMMON_H__
#define __COMMON_H__


#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define SHM_PATH "/tmp/shm4"

struct shm////共享内存使用的结构体的声明 
{
	int a; // 8个字节
	pthread_rwlock_t lock; //这个 56个字节? 不知道,反正占用了 56个字节 // 用来实现互斥的
	volatile sem_t sem1;
	volatile sem_t sem2;//posix 同步
	volatile sem_t sem3;

	int semid;//systemV 信号量同步


};



#endif
