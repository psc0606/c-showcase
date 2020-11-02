#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

typedef struct
{
	char name[4];
	int age;
}people;

int main(int argc, char **argv)
{
	int fd, i;
	people *p_map;
	char temp;
	umask(0000);
	fd = open(argv[1],O_CREAT | O_RDWR | O_TRUNC, 0777);
	lseek(fd,sizeof(people)*10-1,SEEK_SET);
	write(fd,"",1);
	p_map = (people*)mmap(NULL,sizeof(people)*10,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if (p_map == (people*)-1)
	{
		perror("map err");
	}
	close(fd);
	printf("ptr:%x\n",p_map);
	
	temp = 'a';
	for (i = 0; i < 10; i++)
	{
		temp += 1;
		memcpy((*(p_map+i)).name,&temp,2);
		(*(p_map+i)).age = 20 + i;
		printf("%d\n",i);
		sleep(1);
	}
	
	printf("initalize over.\n");
	munmap(p_map,sizeof(people)*10);
	printf("umap ok.\n");
}
