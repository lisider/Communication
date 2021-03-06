/*************************************************************************
    > File Name: client.c
    > Author: Sues
    > Mail: sumory.kaka@foxmail.com 
    > Created Time: 2016年09月29日 星期四 14时19分43秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#define BUFSIZE 128
#define PATH 	"./domainsocket"
#define CLIENT_PATH "./client_domainsocket"

typedef struct sockaddr SA;

int main(int argc, const char *argv[])
{
	int socket_fd;
	struct sockaddr_un server_addr,client_addr;
	socklen_t addr_len = sizeof(server_addr);
	char buf[BUFSIZE];
	int ret = 0;

	socket_fd = socket(PF_UNIX,SOCK_DGRAM,0);
	if(-1 == socket_fd){
		perror("socket created failed");
		exit(-1);
	}


	bzero(&server_addr,sizeof(server_addr));
	server_addr.sun_family = PF_UNIX;
	strcpy(server_addr.sun_path,PATH);

	bzero(&client_addr,sizeof(client_addr));
	client_addr.sun_family = PF_UNIX;
	strcpy(client_addr.sun_path,CLIENT_PATH);
	remove(CLIENT_PATH);

	ret = bind(socket_fd,(SA *)&client_addr,addr_len);
	if(0 > ret){
		perror("bind failed");
		exit(-1);
	}

	strcpy(buf,"hello");
	printf("\n正在发送数据 : %s",buf);
	sendto(socket_fd,buf,BUFSIZE,0,(SA *)&server_addr,addr_len);

	
	puts("\n正在接收数据");
	ret = recvfrom(socket_fd,buf,BUFSIZE,0,(SA *)&server_addr,&addr_len);
	//ret = recvfrom(socket_fd,buf,BUFSIZE,0,NULL,NULL);
	if(0 > ret){
		perror("recvfrom failed");
		exit(0);
	}else{	
		printf("接收到数据 : %s\n",buf);
	}
	close(socket_fd);
	puts("\n程序正在关闭\n");
	return 0;
}
