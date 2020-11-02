#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

void err_sys(const char *msg) {
	perror(msg);
	exit(1);
}

/**
 * 套接字可以用于网络通信，也可以用于本机内的进程通信.
 * 由于本机内进程的IP地址都相同，因此只需要进程号来确定通信的双方.
 * Linux环境下使用socketpair()创造一对未命名的,相互连接的UNIX域套接字.
 */
int main() {
	int sockfd[2];
	pid_t pid;

	//AF_UNIX
	if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) == -1) {
		err_sys("socketpair");
	}
	if((pid = fork()) == -1) {
		err_sys("fork");
	} else if(pid == 0) { //child process
		char s[1024];
		ssize_t n;

		//write port
		close(sockfd[1]); 
		if((n = read(sockfd[0], s, sizeof(s))) < 0) {
			err_sys("read error");
		}
		printf("read:%s\n", s);
		close(sockfd[0]);
		exit(0);
	} else if(pid > 0) { //parent process
		char buf[] = "Hello world!";
		ssize_t n;
		close(sockfd[0]); //read port
		if((n = write(sockfd[1], buf, sizeof(buf))) < 0) {
			err_sys("write error");
		}
		close(sockfd[1]);
		wait(NULL);
		return 0;
	}
}
