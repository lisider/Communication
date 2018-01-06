/*
 *该文件实现了父子进程的通信, 注意 pipe只能用于父子进程?
 *
 *
 * */

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

int main(int argc, const char *argv[])
{

	char buf[64];
	int fd[2];
	pid_t pid;

	bzero(buf,sizeof(buf));

	pipe(fd);
	pid = fork();
	if(0>pid){
		printf("error");
	}else if(0==pid){
		//son
		close(fd[0]);//close read fd
		
		strcpy(buf,"hello");
		printf("son process send a string       : %s\n",buf);
		write(fd[1],buf,strlen(buf));
		return 0;
	}else{
		//father
		close(fd[1]); //close write fd
		read(fd[0],buf,sizeof(buf));
		printf("father process receive a string : %s\n",buf);
		return 0;
	}
}
