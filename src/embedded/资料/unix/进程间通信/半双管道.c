#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd[2], n;
	char c;
	pid_t pid;
	pipe(fd);
	if ((pid = fork()) == 0)
	{
		sleep(3);
		if ((n = read(fd[0],&c,1)) != 1)
		{
			perror("read err");
			exit(1);
		}
		printf("child read :%c\n",c);
		write(fd[1],"c",1);
		exit(0);
	}
	else if (pid > 0)
	{
		write(fd[1],"p",1);
		sleep(3);
		if ((n = read(fd[0],&c,1)) != 1)
		{
			perror("read err");
			exit(1);
		}
		printf("parent read :%c\n",c);
		exit(0);
	}
}
