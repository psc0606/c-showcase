#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_usr(int signo)
{
	printf("alrm\n");
}

int main()
{
	printf("pid :%d\n",getpid());
	if (signal(SIGALRM,sig_usr) == SIG_ERR)
	{
		perror("signal err");
		exit(0);
	}
	pause();
	printf("exit\n");
	exit(0);
}
