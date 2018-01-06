/*************************************************************************
  > File Name: client.c
  > Author: Sues
  > Mail: sumory.kaka@foxmail.com 
  > Created Time: 2016年09月28日 星期三 19时47分34秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <sys/un.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFSIZE 128
#define PATH 	"./domainsocket"
typedef struct sockaddr SA;

int main(int argc, const char *argv[])
{
	int sock_fd;
	int ret = 0;
	char buf[BUFSIZE] = "hello";
	struct sockaddr_un server_addr;

	sock_fd = socket(PF_UNIX,SOCK_STREAM,0);
	if(-1 == sock_fd){
		perror("socket create failed!");
		return -1;
	}
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sun_family = PF_UNIX;
	strcpy(server_addr.sun_path,PATH);

	ret = connect(sock_fd,(SA *)&server_addr,sizeof(server_addr));
	if(-1 == ret){
		perror("connect failed");
		return -1;
	}else{
		printf("\n已连接目标\n");
	}	

	ret = send(sock_fd,buf,BUFSIZE,0);
	if(-1 == ret){
		perror("send failed");
		return -1;
	}

	printf("客户端发送的字符串%s\n",buf);

	bzero(buf,BUFSIZE);

	ret = recv(sock_fd,buf,BUFSIZE,0);
	if(-1 == ret){
		perror("recv failed");
		return -1;
	}

	printf("客户端接收的字符串%s\n",buf);


	close(sock_fd);

	printf("已断开连接\n\n");
	return 0;
}
