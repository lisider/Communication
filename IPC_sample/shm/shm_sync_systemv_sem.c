#include <stdio.h>
#include <sys/ipc.h>
#include <signal.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <unistd.h>
#include "common.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>


struct shm *shms;//结构体指针定义 
int shmid;//共享内存id定义


union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};


int sem_P(int semid, int semnum)
{
	struct sembuf sops={semnum,-1, SEM_UNDO};
	return (semop(semid,&sops,1));
}

/***对信号量数组semnum编号的信号量做V操作***/
int sem_V(int semid, int semnum)
{
	struct sembuf sops={semnum,+1, SEM_UNDO};
	return (semop(semid,&sops,1));
}


void fun_father(void){

	int flag ;

	while(1){

		sleep(1);

		flag = sem_P(shms->semid,0)  ;
		if ( flag )
		{
			perror("P operate error") ;
			exit(-1);
		}
		puts("1");
		puts("2");
		puts("3");
		puts("4");
		if (sem_V(shms->semid, 1) < 0)
		{
			perror("V operate error") ;
			exit(-1) ;
		}

	}

	return ;
}


void fun_son(void){

	int flag ;
	while(1){

		sleep(1);
		flag = sem_P(shms->semid,1)  ;
		if ( flag )
		{
			perror("P operate error") ;
			exit(-1) ;
		}

		puts("5");
		puts("6");
		puts("7");
		puts("8");
		if (sem_V(shms->semid, 2) < 0)
		{
			perror("V operate error") ;
			exit(-1) ;
		}


	}

	return ;
}

void fun_grandson(void){

	int flag ;
	while(1){


		sleep(1);
		flag = sem_P(shms->semid,2)  ;
		if ( flag )
		{
			perror("P operate error") ;
			exit(-1) ;
		}

		puts("9");
		puts("10");
		puts("11");
		puts("12");
		if (sem_V(shms->semid, 0) < 0)
		{
			perror("V operate error") ;
			exit(-1) ;
		}

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
	union semun arg;
	int ret = 0;
	key_t sem_key;

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


	sem_key = ftok("/tmp", 0x66 ) ;
	if ( key < 0 )
	{
		perror("ftok key error") ;
		return -1 ;
	}

	//创建一个信号量集合,集合中有三个信号量
	shms->semid = semget(sem_key,3,IPC_CREAT|0600);
	if (shms->semid == -1)
	{
		perror("create semget error");
		return -1;
	}


	//对 0 号信号量的资源赋值为  1
	arg.val = 1;//这个是1个信号量的初值,标识有几个资源
	/***对0号信号量设置初始值***/
	ret =semctl(shms->semid,0,SETVAL,arg);
	if (ret < 0 )
	{
		perror("ctl sem error");
		semctl(shms->semid,0,IPC_RMID,arg);
		return -1 ;
	}

	//对 1 号信号量的资源赋值为  0
	arg.val = 0;//这个是1个信号量的初值,标识有几个资源
	/***对0号信号量设置初始值***/
	ret =semctl(shms->semid,1,SETVAL,arg);
	if (ret < 0 )
	{
		perror("ctl sem error");
		semctl(shms->semid,0,IPC_RMID,arg);
		return -1 ;
	}

	//对 2 号信号量的资源赋值为  0
	arg.val = 0;//这个是1个信号量的初值,标识有几个资源
	/***对0号信号量设置初始值***/
	ret =semctl(shms->semid,2,SETVAL,arg);
	if (ret < 0 )
	{
		perror("ctl sem error");
		semctl(shms->semid,0,IPC_RMID,arg);
		return -1 ;
	}


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
