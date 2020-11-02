#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

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
	
	char buf[128];
	if (read(sockfd,buf,sizeof(buf)) < 0)
	{
		perror("read err");
		return -1;
	}
	printf("buf:%s\n",buf);
	
	getchar();
}
