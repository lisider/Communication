#include <stdio.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include <unistd.h>
#include <stdlib.h>


struct shm *shms;//结构体指针定义 
int shmid;//共享内存id定义



void fun_father(void){
	int ret = 0;

	while(1){

		sleep(1);
		printf("before wait\n");
		ret = sem_wait(&shms->sem1);
		if(ret < 0){
			printf("sem_wait ret : %d\n",ret);
			perror("sem_wait");
		}

		puts("1");
		puts("2");
		puts("3");
		puts("4");

		ret = sem_post(&shms->sem2);
		if(ret < 0){
			printf("sem_post ret : %d\n",ret);
			perror("sem_post");
		}
		printf("after post\n");
	}

	return ;
}


void fun_son(void){

	while(1){

		sleep(1);
		sem_wait(&shms->sem2);

		puts("5");
		puts("6");
		puts("7");
		puts("8");

		sem_post(&shms->sem1);

	}

	return ;
}

void fun_grandson(void){

	while(1){


		sleep(1);
#if 0
		sem_wait(&shms->sem3);

		puts("9");
		puts("10");
		puts("11");
		puts("12");
		sem_post(&shms->sem1);
#endif
		puts("i am live");

	}

	return ;
}

void sig_handler(int arg){
	switch(arg){                                                                 

		case SIGINT: //退出信号
			pthread_rwlock_destroy(&(shms->lock));
			shmdt(shms);
			shmctl(shmid, IPC_RMID, NULL);
		break;
	}
}


int main(int argc, const char *argv[]){


	key_t key;//key定义
	pid_t pid;

	signal(SIGINT, sig_handler);

	if(access(SHM_PATH,F_OK) != 0)                                                   
	{                                                                                
		printf("mkdir SHM_PATH\n");                                                      
		system("mkdir " SHM_PATH);                                                       
	}  

	key = ftok(SHM_PATH,'r');//获取key
	if(-1 == key){
		perror("ftok");
		return -1; 
	}   
	shmid = shmget(key,sizeof(struct shm),IPC_CREAT|IPC_EXCL|0666);//共享内存的获取
	if(-1 == shmid){
		if(errno == EEXIST){
			shmid = shmget(key,sizeof(struct shm),0);
		}else{
			perror("shmget");
			return -1; 
		}   
	}   

	shms = shmat(shmid,NULL,0);//共享内存的映射
	if(-1 == *(int *)shms){
		perror("shmat");
		return -1; 
	}


	sem_init(&shms->sem1, 0, 1);
	sem_init(&shms->sem2, 0, 0);
	sem_init(&shms->sem3, 0, 0);

	if((pid = fork())<0){
		perror("fork");
	}else if (pid > 0){//father
		fun_father();
	}else{
		if((pid = fork())<0){
			perror("fork");
		}else if (pid > 0){//son
			fun_son();
		}else{// grand son
			fun_grandson();
		}
	}

	return 0;

	//通过最后一个函数(共享内存的映射),shms指针指向的结构体成为共享内存中的结构体.
	//可以通过shms指针来对结构体里面的数据进行读写.
}
