/*
 * 循环同步的话 最好不要找 条件变量 和 锁的 机制来做,容易造成死锁
 * 这个文件就会造成死锁,这个是个错误示例
 *
 *
 * */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_t       pthid1,pthid2,pthid_control;
pthread_cond_t  cond1,cond2;
pthread_mutex_t mutex;

int count = 0;

static void * fun1(void *arg){

	while(1){
		pthread_mutex_lock(&mutex);
		if(count != 0)
			pthread_cond_wait(&cond1,&mutex);
		else 
			count ++;

		puts("first");

		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond2);
	}
	return NULL;
}

static void * fun2(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond2,&mutex);

		puts("second");

		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond1);
	}
	return NULL;
}




int main(int argc, const char *argv[])
{
#if 0 //锁不初始化也没问题
	if(0 != pthread_mutex_init(&mutex,NULL)){
		perror("mutex init");
		return -1;
	}
#endif

	if(0 != pthread_cond_init(&cond1,NULL)){
		perror("cond1 init");
		return -1;
	}
	if(0 != pthread_cond_init(&cond2,NULL)){
		perror("cond2 init");
		return -1;
	}

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
