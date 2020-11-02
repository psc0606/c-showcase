#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char name[4];
	int age;
}people;

int main(int argc, char **argv)
{
	pid_t pid;
	int i;
	people *p_map;
	char temp;
	p_map = (people*)mmap(NULL,sizeof(people)*10,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	if ((pid = fork()) == 0)
	{
		sleep(2);
		for (i = 0; i < 5; i++)
		{
			printf("child read:the %d people's age is %d\n",i+1,p_map[i].age);
		}
		(*p_map).age = 100;
		munmap(p_map,sizeof(people)*10);
		//(*p_map).age = 100;//解除后不执行
		exit(0);
	}
	else if (pid > 0)
	{
		temp = 'a';
		for (i = 0; i < 5; i++)
		{
			temp += 1;
			memcpy(p_map[i].name,&temp,2);
			p_map[i].age = 20 + i;
		}
		sleep(5);
		printf("parent read: the first people is age is %d\n",p_map[0].age);
		munmap(p_map,sizeof(people)*10);
		printf("umap ok.\n");
	}
}
