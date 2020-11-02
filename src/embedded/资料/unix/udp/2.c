#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc,char **argv)//.client ip port
{
	int sockfd;
	struct sockaddr_in addr;
	char buf[1024];
	socklen_t len;
	
	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket err");
		return -1;
	}
	
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&addr.sin_addr.s_addr);
	
	if (sendto(sockfd,"hellow server.",15,0,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		perror("sendto err");
		return -1;
	}
	len = sizeof(struct sockaddr);
	if (recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr*)&addr,&len) < 0)
	{
		perror("recvfrom err");
		return -1;
	}
	getchar();
	close(sockfd);
}
