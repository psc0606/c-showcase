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
		printf("father ipd :%d ,pid :%d\n",getppid(),getpid());
		sleep(3);
		printf("father ipd :%d ,pid :%d\n",getppid(),getpid());
		printf("child exit!\n");
	}
	else if (pid > 0)
	{
		printf("father ipd :%d ,child :%d\n",getpid(),pid);
		sleep(1);
		printf("father exit!\n");
		exit(0);
	}
}
