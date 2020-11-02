#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pc;
	pc = fork();
	if (pc < 0)
	{
		printf("fork err");
	}
	else if (pc == 0)
	{
		printf("this is child process with pidof %d.\n",getpid());
		sleep(3);
		exit(10);
	}
	else
	{
		int status;
		wait(&status);
		if (WIFEXITED(status))
		{
			printf("the child process %d.\nstatus:%d\n",pc,WEXITSTATUS(status));
		}
		else
		{
			printf("the child process %d exit abnormally.\n",pc);
		}
	}
}
