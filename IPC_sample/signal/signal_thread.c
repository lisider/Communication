/*
 *该文件 实现了一个信号对于两个线程的同步,其实信号可以同步两个进程
 *
 *
 * */

#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_t   pthid1,pthid2;

void handler(int arg){
	return;
}

static void * fun1(void *arg){
	sleep(1);
	puts("first");
	pthread_kill(pthid2,SIGUSR1); //发送给线程 2 
	return NULL;
}

static void * fun2(void *arg){

	signal(SIGUSR1,handler);
	pause();
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
