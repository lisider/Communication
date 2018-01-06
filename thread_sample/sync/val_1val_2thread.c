/*
 *这个文件实现了两个线程,3个模块之间的同步 
 *  线程1 中 的  first  与  third 
 *  线程2 中  的  second
 *
 *  同步的顺序 为 first second third
 *
 * */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <unistd.h>
#include <semaphore.h>

pthread_t   pthid1,pthid2;

sem_t       sem;


typedef enum {
	FUN1,
	FUN2
} state_t;
state_t state = FUN1;


typedef int (*sem_fun_t)(sem_t *);
int do_once_val = 0;
void do_once(sem_fun_t fun,sem_t *sem){
	if(do_once_val == 0){
		do_once_val ++;
		fun(sem);
	}
	return ;
}


static void * fun1(void *arg){
	while(1){
		puts("first");
		state = FUN2;
		do_once(sem_post,&sem);
loop1:
		if(FUN1 == state){
			puts("third");
		}else
			goto loop1;
	}
	return NULL;
}


static void * fun2(void *arg){
	sem_wait(&sem);
	while(1){
loop2:
		if(FUN2 == state){
			state = FUN1;
			puts("second");
		}else
			goto loop2;
	}
	return NULL;
}



int main(int argc, const char *argv[])
{
	sem_init(&sem, 0, 0);

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
