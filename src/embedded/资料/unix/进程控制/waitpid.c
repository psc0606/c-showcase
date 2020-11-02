#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork err");
		return -1;
	}
	else if (pid == 0)
	{
		sleep(3);
		printf("child 1 %d exit!\n",getpid());
	}
	else if (pid > 0)
	{
		pid_t pid1;
		pid1 = fork();
		if (pid1 < 0)
		{
			perror("fork err");
			exit(1);
		}
		else if (pid1 == 0)
		{
			sleep(1);
			printf("child %d exit.\n",getpid());
			exit(2);
		}
		else
		{
			int status;
			printf("father start!\n");
			//waitpid(pid,&status,0);
			//sleep(1);
			printf("wait pid:%d\n",waitpid(pid,&status,0));
			printf("father exit!\n");
			exit(0);
		}
	}
}
