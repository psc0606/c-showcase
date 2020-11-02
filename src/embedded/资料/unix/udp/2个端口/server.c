#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)//./server port
{
	int sockfd1, sockfd2, client_sockfd, len, maxfd;
	char ip[32];
	fd_set fds;
	struct timeval tm;
	
	struct sockaddr_in ser_addr,client_addr;
	
	if ((sockfd1 = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket err");
		return -1;
	}
	
	if ((sockfd2 = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket err");
		return -1;
	}
	
	bzero(&ser_addr,sizeof(struct sockaddr));
	
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(atoi(argv[1]));
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(sockfd1,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr)) < 0)
	{
		perror("bind 1 err");
		return -1;
	}
	
//	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(atoi(argv[2]));
//	ser_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd2,(struct sockaddr*)&ser_addr,sizeof(struct sockaddr)) < 0)
	{
		perror("bind 2 err");
		return -1;
	}
	
	if (listen(sockfd1,5) < 0)
	{
		perror("listen err");
		return -1;
	}
	
	if (listen(sockfd2,5) < 0)
	{
		perror("listen err");
		return -1;
	}
	
	FD_ZERO(&fds);
	//FD_SET(sockfd1,&fds);
	//FD_SET(sockfd2,&fds);
	maxfd = sockfd1 > sockfd2? sockfd1:sockfd2;
	
	int res;
	while (1)
	{
		FD_SET(sockfd1,&fds);
		FD_SET(sockfd2,&fds);
		tm.tv_sec = 3;
		tm.tv_usec = 100;
		res = select(maxfd+1,&fds,NULL,NULL,&tm);
		/*if (select(maxfd+1,&fds,NULL,NULL,&tm) < 0)
		{
			perror("select err");
			return -1;
		}*/
		if (res < 0)
		{
			perror("res < 0");
			return -1;
		}
		if (res == 0)
		{
			printf("no connect\n");
			continue;
		}
		if (FD_ISSET(sockfd1,&fds))
		{
			fun(sockfd1);
		}
		if (FD_ISSET(sockfd2,&fds))
		{
			fun(sockfd2);
		}
	}
	getchar();
}

int fun(int sockfd)
{
	int client_sockfd;
	struct sockaddr_in client_addr;
	socklen_t len;
	char ip[32];
	
	bzero(&client_addr,sizeof(client_addr));
	len = sizeof(client_addr);
	if ((client_sockfd = accept(sockfd,(struct sockaddr*)&client_addr,&len)) < 0)
	{
		perror("accept err");
		return -1;
	}
	
	inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ip,sizeof(ip));
	printf("%s\n",ip);
	time_t tm;
	time(&tm);
	char *ptr = ctime(&tm);
	if (write(client_sockfd,ptr,strlen(ptr)+1) != strlen(ptr)+1)
	{
		perror("write err");
		return -1;
	}
	
	return 0;
}
