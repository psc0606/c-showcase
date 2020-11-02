#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork err!\n");
	}
	else if (pid == 0)
	{
		exit(0);
	}
	else
	{
		sleep(60);
	}
}
