#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

void sig_alarm(int signo)
{
	int i;
	time_t tm;
	for (i = 0; i < 10; i++)
	{
		time(&tm);
		printf("%s",ctime(&tm));
		sleep(1);
	}
}

int main()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sig_alarm;
	if (sigaction(SIGALRM, &sig_act,NULL) < 0)
	{
		perror("sigaction err");
		exit(0);
	}
	raise(SIGALRM);
	printf("pid : %d\n",getpid());
	//pause();
	printf("exit\n");
}
