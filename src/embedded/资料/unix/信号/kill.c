#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		int ppid = getppid();
		if (kill(ppid,SIGALRM) < 0)
		{
			perror("pid 1 err");
		}
		printf("child exit\n");
		exit(0);
	}
	else if (pid > 0)
	{
		printf("wait pid %d",waitpid(pid,NULL,0));
		printf("father eixt\n");
	}
}
