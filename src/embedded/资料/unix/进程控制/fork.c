#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int glob = 6;
char buf[] = "a write to stdout\n";
int main()
{
	int var = 30;
	pid_t pid;
//	pid = fork();
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
	{
		perror("write failed");
		return -1;
	}
	if ((pid = fork()) < 0)
	{
		perror("err");
		return -1;
	}
	else if (pid == 0)
	{
		glob++;
		var++;
	}
	else
	{
		sleep(2);
	}
	printf("pid = %d, glob = %d, var = %d\n",getpid(),glob,var);
}

/*
int main()
{
	int i = 0;
	pid_t result;
	if ((result = fork()) == -1)
	{
		perror("fork err");
		return -1;
	}
	else if (result == 0)
	{
		for (i = 0; i < 5; i++)
		{
			printf("In child process!\n");
			sleep(1);
		}
		exit(0);
	}
	else
	{
		for (i = 0; i < 5; i ++)
		{
			printf("In father process!\n");
			sleep(1);
		}
		exit(0);
	}
}
*/
