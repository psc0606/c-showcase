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

int main(int argc, char **argv)//./server port
{
	int sockfd, client_sockfd, len;
	struct sockaddr_in ser_addr,client_addr;
	
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket err");
		return -1;
	}
	
	bzero(&ser_addr,sizeof(struct sockaddr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(atoi(argv[1]));
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr)) < 0)
	{
		perror("bind err");
		return -1;
	}
	
	if (listen(sockfd,5) < 0)
	{
		perror("listen err");
		return -1;
	}
	
	bzero(&client_addr,sizeof(client_addr));
	len = sizeof(client_addr);
	if ((client_sockfd = accept(sockfd,(struct sockaddr*)&client_addr,&len)) < 0)
	{
		perror("accept err");
		return -1;
	}
	
//	inet_ntop(AF_INET,&client_addr.sin_addr,ip,sizeof(ip));
//	printf("%s\n",ip);

	if (pthread_create(&tid1,NULL,fun1,&client_sockfd) != 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if (pthread_create(&tid2,NULL,fun2,&client_sockfd) != 0)
	{
		perror("create pthread 2 err");
		exit(0);
	}

	pthread_join(tid2,NULL);
	pthread_cancel(tid1);
}
