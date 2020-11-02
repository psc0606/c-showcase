#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc,char **argv)//./udp port
{
	int sockfd;
	struct sockaddr_in ser_addr,client_addr;
	char buf[1024];
	socklen_t len;
	char ip[32];
	
	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socker err");
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
	
	len = sizeof(struct sockaddr);
	if (recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&len) < 0)
	{
		perror("recvfrom err");
		return -1;
	}
	
	inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ip,sizeof(ip));
	printf("ip:%s say %s\n",ip,buf);
	
	if (sendto(sockfd,"hello client.",15,0,(struct sockaddr*)&client_addr,sizeof(struct sockaddr)) < 0)
	{
		perror("send to err");
		return -1;
	}
	getchar();
}
