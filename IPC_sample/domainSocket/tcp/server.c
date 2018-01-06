/*************************************************************************
    > File Name: server.c
    > Author: Sues
    > Mail: sumory.kaka@foxmail.com 
    > Created Time: 2016年09月28日 星期三 22时44分03秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH 	"./domainsocket"
#define BUFSIZE 128


typedef struct sockaddr SA;

int main(int argc, const char *argv[])
{
	int socket_fd,accept_fd;
	int ret = 0;
	socklen_t addr_len;
	char buf[BUFSIZE];
	struct sockaddr_un server_addr;

	socket_fd = socket(PF_UNIX,SOCK_STREAM,0);
	if(-1 == socket_fd){
		perror("socket create failed!");
		return -1;
	}
	remove(PATH);
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sun_family = PF_UNIX;
	strcpy(server_addr.sun_path,PATH);

	ret = bind(socket_fd,(SA *)&server_addr,sizeof(server_addr));
	if(0 > ret){
		perror("bind failed");
		return -1;
	}
	
	ret = listen(socket_fd,10);
	if(-1 == ret){
		perror("listen failed");
		return -1;
	}

	while(1){
		puts("\n正在等待目标连接...");
		accept_fd = accept(socket_fd,NULL,NULL);
		if(0 > accept_fd){
			perror("accept failed");
			return -1;
		}else{
			puts("\n已连接目标");
		}
		
		bzero(buf,BUFSIZE);
		
		ret = recv(accept_fd,buf,BUFSIZE,0);

		if(-1 == ret){
			perror("recv failed");
			return -1;
		}
		
		printf("服务器接收的字符串%s\n",buf);

		send(accept_fd,buf,BUFSIZE,0);

		printf("服务器发送的字符串%s\n",buf);

		close(accept_fd);
		
		printf("已断开连接\n");
	}

	close(socket_fd);
	return 0;
}
