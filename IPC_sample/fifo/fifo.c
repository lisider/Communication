/*************************************************************************
    > File Name: fifo.c
    > Author: Sues
    > Mail: sumory.kaka@foxmail.com 
    > Created Time: 2016年10月13日 星期四 19时54分40秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define FIFO_PATH "/tmp/fifo_test"

int main(int argc, const char *argv[])
{
	int ret;
	int fd;
	pid_t pid;

	char buf[32];	

	bzero(buf,sizeof(buf));


	if((access(FIFO_PATH,F_OK))!=-1) 
	{
		strcpy(buf,"rm -f ");
		strcat(buf,FIFO_PATH);
		system(buf);
	}

	bzero(buf,sizeof(buf));
	ret = mkfifo(FIFO_PATH,0664);
	if(-1 == ret){
		perror("mkfifo");
	}

	fd = open(FIFO_PATH,O_RDWR);
	if(0 > fd){
		perror("open");
	}

	pid = fork();
	if(0 > fork){
		perror("fork");
	}else if(0 == pid){
		//son
		read(fd,buf,sizeof(buf));
		printf("%s\n",buf);
		exit(23);
	}else{
		//father
		int status = 0;
		int pid_dead = 0;
		strcpy(buf,"hello");
		write(fd,buf,sizeof(buf));

		sleep(1);


		pid_dead = waitpid(pid, &status,WNOHANG);

		printf("wait process %d, process %d exit value is %d\n",pid,pid_dead,status); //为什么打印的值不是 23?
	//	wait(&status);
	//	printf("process %d exit value is %d\n",pid,status);
		
		exit(0);
	}
	return 0;
}
