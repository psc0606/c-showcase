/*
 * server for Network IPC
 */
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
	int sockfd, client_sockfd;
      	int res = 0, len;
	char buf[128];
	struct sockaddr_in addr;

	/* create socket for server */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		perror("socket");
		return -1;
	}

	/* address for client */
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	addr.sin_port = htons(8080);

	/* bind server's socket with address*/
	res = bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
	if(res < 0) {
		perror("bind");
		goto close_sockfd;
	}

	res = listen(sockfd, 5);
	if(res < 0) {
		perror("listen");
		goto close_sockfd;
	}

	len = sizeof(struct sockaddr_in);
	client_sockfd = accept(sockfd, (struct sockaddr *)&addr, &len);
	if(client_sockfd < 0) {
		perror("accept");
		goto close_sockfd;
	}
	
	bzero(buf, sizeof(buf));
	read(client_sockfd, buf, sizeof(buf));
	puts(buf);

	close(client_sockfd);
	close(sockfd);
	return 0;

close_sockfd:
	close(sockfd);
	return -1;
}
