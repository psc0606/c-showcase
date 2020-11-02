#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

typedef struct
{
	char name[4];
	int age;
}people;

int main(int argc,char **argv)
{
	int shm_id, i;
	key_t key;
	char temp;
	people *p_map;
	key = ftok(argv[1],2);
	if (key == -1)
	{
		perror("ftok err");
		exit(0);
	}
	shm_id = shmget(key,4096,IPC_CREAT | 0666);
	if (shm_id == -1)
	{
		perror("shmget err");
		exit(0);
	}
	p_map = (people*)shmat(shm_id,NULL,0);
	temp = 'a';
	for (i = 0; i < 10; i++)
	{
		temp += 1;
		memcpy((*(p_map+i)).name,&temp,1);
		(*(p_map+i)).age = 20+i;
	}
	if (shmdt(p_map) == -1)
	{
		perror("detach err");
	}
}
