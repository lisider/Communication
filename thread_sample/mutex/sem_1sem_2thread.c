/*
 * 这里面用1个信号量实现了两个线程的互斥
 * fun1 和fun2 分别表示为 1个模块
 * 执行顺序不定,但是两个模块不会同时执行
 * */
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_t   pthid1,pthid2;
sem_t       sem;
static int i = 1000;

static void * fun1(void *arg){
	while(1){
		sem_wait(&sem);
        printf("thread1 --- %d\n",--i);
        usleep(1000);
        printf("thread1 --- %d\n",++i);
		sem_post(&sem);
	}
	return NULL;
}

static void * fun2(void *arg){
	while(1){
		sem_wait(&sem);
        printf("thread2 --- %d\n",--i);
        usleep(1000);
        printf("thread2 --- %d\n",++i);
		sem_post(&sem);
	}
	return NULL;
}

int main(int argc, const char *argv[])
{
	sem_init(&sem, 0, 1);

	if(0 != pthread_create(&pthid1,NULL,fun1,NULL)){
		perror("pthid1");
		return -1;
	}
	if(0 != pthread_create(&pthid2,NULL,fun2,NULL)){
		perror("pthid2");
		return -1;
	}

	pthread_join(pthid1,NULL);
	pthread_join(pthid2,NULL);
    sem_destroy(&sem);

	return 0;
}
