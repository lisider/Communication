/*
 * 循环同步的话 最好不要找 条件变量 和 锁的 机制来做,容易造成死锁
 * 非循环同步,可以用条件变量和锁的机制来做,没什么问题
 *
 *
 * */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_t       pthid1,pthid2;

int count = 0;

static void * fun1(void *arg){


	puts("first");

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
