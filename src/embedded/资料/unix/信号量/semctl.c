#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *_buf;
};

int main()
{
	key_t key;
	key = ftok(".",1);
	int semid, val, i;
	semid = semget(key,3,IPC_CREAT | 0644);
	printf("pid: %d\n",getpid());

	union semun sem;
	sem.val = 10;
	semctl(semid,2,SETVAL,sem);
	sem.array = malloc(sizeof(int)*3);
	val = semctl(semid,0,GETALL,sem);

	for (i = 0; i < 3; i++)
	{
		printf("i:%d\tval:%d\n",i,sem.array[i]);
	}
}
