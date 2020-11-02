#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
	key_t key;
	key = ftok(".",1);
	int semid;
	semid = semget(key,1,IPC_CREAT | 0644);	
}
