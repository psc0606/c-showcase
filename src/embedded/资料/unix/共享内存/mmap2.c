#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

typedef struct
{
	char name[4];
	int age;
}people;

int main(int argc,char **argv)
{
	int fd, i;
	people *p_map;
	if ((fd = open(argv[1],O_RDWR)) < 0)
	{
		perror("open failed");
		exit(1);
	}
	p_map = (people*)mmap(NULL,sizeof(people)*10,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if (p_map == (people*) -1)
	{
		perror("mmap");
		exit(1);
	}
	printf("ptr:%x\n",p_map);
	for (i = 0; i < 10; i++)
	{
		printf("name:%s age:%d;\n",(*(p_map+i)).name,p_map[i].age);
	}
}
