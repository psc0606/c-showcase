#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)//./server port
{
	int sockfd, client_sockfd, len;
	char ip[32];
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
	
	inet_ntop(AF_INET,&client_addr.sin_addr,ip,sizeof(ip));
	printf("%s\n",ip);
	
	time_t tm;
	time(&tm);
	char *ptr = ctime(&tm);
	
	if (write(client_sockfd,ptr,strlen(ptr)+1) < 0)
	{
		perror("write err");
		return -1;
	}
	getchar();
}
