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

#if 0
	ret = mkfifo(FIFO_PATH,0664);
	if(-1 == ret){
		perror("mkfifo");
	}
#endif

	fd = open(FIFO_PATH,O_RDWR);
	if(0 > fd){
		perror("open");
	}

	strcpy(buf,"hello");
	write(fd,buf,sizeof(buf));

	printf("send a string :%s",buf);
		
	return 0;
}
