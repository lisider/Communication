/*
 *这个文件实现了一个信号量对两个线程 的同步,但是同步只有一次 ,不能循环同步
 * fun1 和fun2 分为为两个模块
 * 同步顺序 为 fun1 fun2
 *
 *
 * */
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

pthread_t   pthid1,pthid2;

static void * fun1(void *arg){

	puts("fist");
	return NULL;
}

static void * fun2(void *arg){
	puts("second");
	return NULL;
}

int main(int argc, const char *argv[])
{

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

	return 0;
}
