#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define MAXSIZE 1024

int main()
{
	int shmid;
	key_t key;
	key = ftok(".",1);
	umask(0000);
	shmid = shmget(key,MAXSIZE,IPC_CREAT | 0644);

	if (shmid < 0)
	{
		perror("shmget err");
		exit(1);
	}

	char *ptr;
	/*
	ptr = shmat(shmid,NULL,0);
	printf("ptr:%x\n",ptr);
	
	//scanf("%s",ptr);
	//strcpy(ptr,"1111");
	
	printf("%s\n",ptr);
	*/
	struct shmid_ds buf;
	
	shmctl(shmid,IPC_STAT,&buf);
	shmdt(ptr);
	printf("%d,%ld\n",buf.shm_perm,buf.shm_segsz);
	exit(0);
}
