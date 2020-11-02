#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

int main()
{
	pid_t pid;
	int i = 0;
	int semid1 = init_sem(0);
	int semid2 = init_sem(1);
	pid = fork();
	if (pid == 0)
	{
		while (i++ < 5)
		{
			sem_p(semid2);
			printf("1111\n");
			sem_v(semid1);
			sleep(1);
		}

	}
	else if (pid > 0)
	{
		sem_p(semid1);
		while (i++ < 5)
		{
			sem_p(semid1);
			printf("2222\n");
			sem_v(semid2);		
			sleep(1);
		}
		wait(NULL);
	}
}
