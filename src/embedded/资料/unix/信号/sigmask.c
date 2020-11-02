#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_usr(int signo)
{
	printf("caught SIGQUIT\n");
	if (signal(SIGQUIT,SIG_DFL) == SIG_ERR)
	{
		perror("reset SIGQUIT err");
		exit(0);
	}
}

int main()
{
	sigset_t newset, oldset, pendset;
	if (signal(SIGQUIT,sig_usr) == SIG_ERR)
	{
		perror("signal quit err");
		exit(0);
	}
	sigemptyset(&newset);
	sigaddset(&newset,SIGQUIT);
	if (sigprocmask(SIG_BLOCK,&newset,&oldset) < 0)
	{
		perror("SIG_BLOCK err");
		exit(0);
	}
	sleep(5);
	if (sigpending(&pendset) < 0)
	{
		perror("sigpendmask err");
	}
	if (sigismember(&pendset,SIGQUIT))
	{
		printf("\nSIGQUIT pending\n");
	}
	if (sigprocmask(SIG_SETMASK,&oldset,NULL) < 0)
	{
		perror("SIG_SETMASK err\n");
	}
	printf("SIGQUIT unblock\n");
	sleep(5);
	exit(0);
}
