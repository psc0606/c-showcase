/*1. 使用socket套接字，编写服务器，客户端，使得它们可以进行简单的聊天。*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

pthread_t tid1,tid2;

void *fun1(void *arg)
{
	int client_sockfd = *(int *)arg;
	char buf[128];
	while (1)
	{
		bzero(&buf,sizeof(buf));
		if (read(client_sockfd,buf,sizeof(buf)) < 0)
		{
			perror("read err");
			exit(0);
		}
		if (strcmp("quit",buf) == 0)
		{
			pthread_cancel(tid2);
			break;
		}
		printf("%s\n",buf);
	}
}

void *fun2(void *arg)
{
	int sockfd = *(int *)arg;	
	char buf[128];
	while (1)
	{
		bzero(&buf,sizeof(buf));
		scanf("%s",buf);	
		if (write(sockfd,buf,strlen(buf)) < 0)
		{
			perror("write err");
			exit(0);
		}
		if (strcmp("quit",buf) == 0)
		{
			break;
		}
	}
}

int main(int argc, char **argv)//client ip port
{
	int sockfd;
	struct sockaddr_in addr;
	
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket err");
		return -1;
	}
	
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&addr.sin_addr.s_addr);
		
	if (connect(sockfd,(struct sockaddr*)&addr,sizeof(struct sockaddr)) < 0)
	{
		perror("connect err");
		return -1;
	}
	
	if (pthread_create(&tid1,NULL,fun1,&sockfd) != 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if (pthread_create(&tid2,NULL,fun2,&sockfd) != 0)
	{
		perror("create pthread 2 err");
		exit(0);
	}

	pthread_join(tid2,NULL);
	pthread_cancel(tid1);
}
