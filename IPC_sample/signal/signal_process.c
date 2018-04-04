/*
 *该文件 实现了一个信号对于两个线程的同步,其实信号可以同步两个进程
 *
 *
 * */

#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pid_t pid;

void handler(int arg){
	return;
}

static void * fun_father(){
	sleep(1);
	puts("first");
	kill(pid,SIGUSR1); //发送给线程 2 

	return NULL;
}

static void * fun_son(){

	signal(SIGUSR1,handler);
	pause();
	puts("second");

	return NULL;
}


int main(int argc, const char *argv[])
{
	if((pid = fork())< 0){
		perror("fork");
	}else if(pid > 0){
		fun_father();
	}else{
		fun_son();
	}
	return 0;
}
