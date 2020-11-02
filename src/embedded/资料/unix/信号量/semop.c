#include "semop.h"

int main()
{
	int semid, i = 0;
	semid = init_sem();
	
	key_t key;
	key = ftok(".",1);

	semid = semget(key,1,IPC_CREAT | 0644);
	pid_t pid = fork();
	
	if (pid == 0)
	{
		while (i++ < 20)
		{
			sem_v(semid);
			printf("C\n");
		}
	}
	else if (pid > 0)
	{
		while (i++ < 20)
		{
			sem_p(semid);
			printf("\tF\n");
		}
		wait(NULL);
	}
}
