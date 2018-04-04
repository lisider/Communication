/*
 * 这里面创建了两个进程 , 用进程间的信号量 实现了互斥
 * 至于同步,怎么同步,有待研究
 * */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

int semid;
union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};
/***对信号量数组semnum编号的信号量做P操作***/
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
	int flag;
		while(1){

			sleep(1);
			flag = sem_P(semid,0)  ;
			if ( flag )
			{
				perror("P operate error") ;
				return -1 ;
			}
			printf("1111111111111\n");
			printf("1111111111111\n");
			printf("1111111111111\n");
			printf("1111111111111\n");
			printf("1111111111111\n");
			printf("1111111111111\n");



			if (sem_V(semid, 0) < 0)
			{
				perror("V operate error") ;
				return -1 ;
			}
		}

}


void fun_son(void){
	int flag;
		while(1){

			sleep(1);
			flag = sem_P(semid,0)  ;
			if ( flag )
			{
				perror("P operate error") ;
				return -1 ;
			}
#if 0
			printf("P operate end\n") ;
			ret =semctl(semid,0,GETVAL,arg);
			printf("after P sem[0].val=[%d]\n",ret);
			system("date") ;
			if ( argc == 1 )
			{
				sleep(120) ;
			}
			printf("V operate begin\n") ;
#endif

			printf("222222222222222222\n");
			printf("222222222222222222\n");
			printf("222222222222222222\n");
			printf("222222222222222222\n");
			printf("222222222222222222\n");
			printf("222222222222222222\n");



			if (sem_V(semid, 0) < 0)
			{
				perror("V operate error") ;
				return -1 ;
			}
		}


}

int main(int argc, char **argv)
{
	int key ;
	int ret;
	union semun arg;
	struct sembuf semop;
	int flag ;

	key = ftok("/tmp", 0x66 ) ;
	if ( key < 0 )
	{
		perror("ftok key error") ;
		return -1 ;
	}
	/***本程序创建了一个信号量**/
	semid = semget(key,1,IPC_CREAT|0600);
	if (semid == -1)
	{
		perror("create semget error");
		return -1;
	}
	arg.val = 1;//这个是1个信号量的初值,标识有几个资源
	/***对0号信号量设置初始值***/
	ret =semctl(semid,0,SETVAL,arg);
	if (ret < 0 )
	{
		perror("ctl sem error");
		semctl(semid,0,IPC_RMID,arg);
		return -1 ;
	}
#if 1
	/***取0号信号量的值***/
	ret =semctl(semid,0,GETVAL,arg);
	printf("after semctl setval  sem[0].val =[%d]\n",ret);
	system("date") ;
	printf("P operate begin\n") ;
#endif

	ret = fork();
	if(ret < 0){
		perror("fork");
	}else if(ret > 0){ //father
		fun_father();

	}else{//son
		fun_son();
	}
#if 0
	printf("V operate end\n") ;
	ret =semctl(semid,0,GETVAL,arg);
	printf("after V sem[0].val=%d\n",ret);
	system("date") ;
	if ( argc >1 )
	{
		semctl(semid,0,IPC_RMID,arg);
	}
#endif

	return 0 ;
}
