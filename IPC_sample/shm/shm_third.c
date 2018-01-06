#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"


struct shm *shms;//结构体指针定义 



void fun_main(void){

	while(1){

		sleep(1);

		pthread_rwlock_rdlock(&(shms->lock));

		puts("write3");
		puts("write3");
		puts("write3");
		puts("write3");

		pthread_rwlock_unlock(&(shms->lock));
	}

	return ;
}



int main(int argc, const char *argv[]){


	key_t key;//key定义
	int shmid;//共享内存id定义

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


	fun_main();



	return 0;

	//通过最后一个函数(共享内存的映射),shms指针指向的结构体成为共享内存中的结构体.
	//可以通过shms指针来对结构体里面的数据进行读写.
}
