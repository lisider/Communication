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
#include <sys/ipc.h>
#include <sys/shm.h>


#define FIFO_PATH "/tmp/fifo_test"

int main(int argc, const char *argv[])
{
	int ret;
	int fd;
	pid_t pid;

	char buf[32];	

	bzero(buf,sizeof(buf));


	if((access(FIFO_PATH,F_OK))!=-1)  //存在
	{
		strcpy(buf,"rm -f ");
		strcat(buf,FIFO_PATH);
		system(buf);
	}

	ret = mkfifo(FIFO_PATH,IPC_CREAT|IPC_EXCL|0666);
	if(-1 == ret){
		perror("mkfifo");
	}

	fd = open(FIFO_PATH,O_RDWR);
	if(0 > fd){
		perror("open");
	}

	read(fd,buf,sizeof(buf));

	printf("recv a string :%s",buf);
		
	return 0;
}
