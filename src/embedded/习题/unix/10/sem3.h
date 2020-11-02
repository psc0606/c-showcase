#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int init_sem(int value,int val)
{
	int semid;
	key_t key;
	if ((key = ftok(".",value)) < 0)
	{
		perror("ftok err");
		return -1;
	}
	if ((semid = semget(key,value,IPC_CREAT | 0644)) < 0)
	{
		perror("semget err");
		return -1;
	}
	
	semctl(semid,0,SETVAL,val);
	return semid;
}

int sem_p(int semid)
{
	struct sembuf sem;
	sem.sem_num = 0;
	sem.sem_op = -1;
	//sem.sem_flg = SEM_UNDO;
	semop(semid,&sem,1);
	return 0;
}

int sem_v(int semid)
{
	struct sembuf sem;
	sem.sem_num = 0;
	sem.sem_op = 1;
	//sem.sem_flg = SEM_UNDO;
	semop(semid,&sem,1);
	return 0;
}
