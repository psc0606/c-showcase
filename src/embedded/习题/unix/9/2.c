/*----------------------------------------
**	2. 主线程产生两个线程，一个线程统计文
**	件中各种数据的个数，一个线程负责从文件
**	中查找对应的信息。提示：使用信号量控制
**	线程的同步。
**----------------------------------------
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "sem.h"

char buf[1024];
int num_a = 0;
int num_A = 0;
int number = 0;
int flag = 0;
int semid1, semid2;

void *fun1(void *arg)
{
	int i;
	sem_p(semid1);
	for (i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] >= 'a' && buf[i] <= 'z')
		{
			num_a++;
		}
		else if (buf[i] >= 'A' && buf[i] <= 'Z')
		{
			num_A++;
		}
		else if (buf[i] >= '0' && buf[i] <= '9')
		{
			number++;
		}
	}
	sem_v(semid2);
}

void *fun2(void *arg)
{
	int i;
	char *b = (char*)arg;
	sem_p(semid2);
	printf("111:%s\n",b);
	for (i = 0; buf[i] != '\0'; i++)
	{
		if (strncmp(buf+i,b,strlen(b)) == 0)
		{
			flag = 1;
		}
	}
}

int main(int argc,char **argv)
{
	int fd, n;
	pthread_t tid1, tid2;
	semid1 = init_sem(1);
	semid2 = init_sem(2);
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if ((fd = open(argv[1],O_RDONLY)) < 0)
	{
		perror("open err");
		exit(0);
	}
	if ((pthread_create(&tid1,NULL,fun1,NULL)) < 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if ((pthread_create(&tid2,NULL,fun2,argv[2])) < 0)
	{
		perror("create pthread 2 err");
		exit(0);
	}
	while (1)
	{
		if ((n = read(fd,buf,sizeof(buf))) == 0)
		{
			break;
		}
		sem_v(semid1);
	}
	pthread_join(tid2,NULL);
	printf("the num_a = %d\n",num_a);
	printf("the num_A = %d\n",num_A);
	printf("the number = %d\n",number);
	if (flag)
	{
		printf("find the word. %s\n",argv[2]);
	}
	else
	{
		printf("no find the word %s\n",argv[2]);
	}
}
