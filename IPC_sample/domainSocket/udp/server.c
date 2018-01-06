/*************************************************************************
    > File Name: server.c
    > Author: Sues
    > Mail: sumory.kaka@foxmail.com 
    > Created Time: 2016年09月29日 星期四 15时25分22秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define BUFSIZE 128
#define PATH 	"./domainsocket"


typedef struct sockaddr SA;


int main(int argc, const char *argv[])
{
	int socket_fd;
	int ret = 0;
	char buf[BUFSIZE];
	struct sockaddr_un server_addr,client_addr;
	socklen_t addr_len = sizeof(server_addr);


	socket_fd = socket(PF_UNIX,SOCK_DGRAM,0);
	if(-1 == socket_fd){
		perror("socket created failed");
		exit(-1);
	}
	
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sun_family = PF_UNIX;
	strcpy(server_addr.sun_path,PATH);

	remove(PATH);

	ret = bind(socket_fd,(SA *)&server_addr,addr_len);
	if(0 > ret){
		perror("bind failed");
		exit(-1);
	}

	//alarm(6);

	bzero(buf,BUFSIZE);
	while(1){
		puts("\n正在接收数据\n");
		ret = recvfrom(socket_fd,buf,BUFSIZE,0,(SA *)&client_addr,&addr_len);
		if(0 > ret){
			perror("recvfrom failed");
			exit(-1);
		}else{
			printf("接收到数据 : %s\n",buf);
		}
		bzero(buf,BUFSIZE);
		strcpy(buf,"hello,too");
		printf("正在发送数据 : %s\n",buf);
		sendto(socket_fd,buf,BUFSIZE,0,(SA *)&client_addr,addr_len);
	}
	close(socket_fd);
	return 0;
}
