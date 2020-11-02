/*
 * client for Network IPC
 */
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <malloc.h>
#include <unistd.h>

int main()
{
	int sockfd;
	int res;
	const int SIZE = 1024*1024*10;
	char *buf = (char*)malloc(SIZE); //10M
	struct sockaddr_in addr;

	/* create a socket for client */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("socket");
		return -1;
	}

	/* name the socket, as agreed with server */
	bzero(&addr, sizeof(struct sockaddr_in));
	//transfer string addr to binary addr
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr); 
	addr.sin_port = htons(8080);

	/* connect socket to server socket */
	res = connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	
	if(res < 0) {
		perror("connect");
		goto close_sockfd;
	}

	/* fill content */
	memset(buf, 'A', SIZE);

	/* write and read to/from server */
	write(sockfd, buf, SIZE);

	close(sockfd);
	return 0;
	
close_sockfd:
	close(sockfd);
	return -1;
}
