/*2. 利用线程拷贝文件。
提示：第一个线程负责读文件内容到缓冲区，第二个线程负责讲缓冲区数据写入文件，第三个线程打印出拷贝进度(已经拷贝数据占整个文件的大小的百分比)。要求使用互斥*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include "sem.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int n = 0, size = 0;
char buf[5];
int semid1, semid2;

void *fun1(void *arg)
{
	int fd = *(int*)arg;
	while (1)
	{
		sem_p(semid1);
		memset(buf,0,sizeof(buf));
		if ((n = read(fd,buf,sizeof(buf))) <= 0)
		{
			break;
		}
		size += n;
		sleep(1);
		sem_v(semid2);
	}
}

void *fun2(void *arg)
{
	int fd = *(int*)arg;
	while (1)
	{
		sem_p(semid2);
		if ((write(fd,buf,n)) <= 0)
		{
			break;
		}
		sem_v(semid1);	
	}
}

void *fun3(void *arg)
{
	int ssize = *(int*)arg;
	do
	{
		printf("%d/%d,%.2f%\n",size,ssize,1.0*size/ssize*100);
		sleep(1);
	}while (size != ssize);
}

int main(int argc,char **argv)
{
	int fd1, fd2;
	pthread_t tid1,tid2,tid3;
	struct stat buf;
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if (stat(argv[1],&buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	if ((fd1 = open(argv[1],O_RDONLY | O_NONBLOCK)) < 0)
	{
		perror("open 1 err");
	}
	umask(0000);
	if ((fd2 = open(argv[2],O_CREAT | O_RDWR | O_TRUNC,0666)) < 0)
	{
		perror("open 2 err");
		exit(0);
	}
	semid1 = init_sem(1);
	semid2 = init_sem(2);
	sem_p(semid2);
	if (pthread_create(&tid1,NULL,fun1,&fd1) < 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if (pthread_create(&tid2,NULL,fun2,&fd2) < 0)
	{
		perror("create pthread 2 err");
		exit(0);
	}
	if (pthread_create(&tid3,NULL,fun3,&buf.st_size) < 0)
	{
		perror("create pthread 3 err");
		exit(0);
	}
	
	pthread_join(tid1,NULL);
	pthread_cancel(tid2);
	pthread_join(tid3,NULL);
}
