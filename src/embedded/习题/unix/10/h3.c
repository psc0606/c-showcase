/*3. 有一个有限缓冲区和两个线程：生产者和消费者。他们分别把产品放入缓冲区和从缓冲区中拿走产品。当一个生产者在缓冲区满时必须等待，当一个消费者在缓冲区空时页必须等待。

用信号量来解决生产者消费者问题中的同步和互斥问题。提示：
这里使用3个信号量，其中两个信号量avail和full分别用于解决生产者和消费者线程之间的同步问题，mutex是用于这两个线程之间的互斥问题。其中avail初始化为N（有界缓冲区的空单元数），mutex初始化为1，full初始化为0。*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sem3.h"

#define N 5

int full, mutex, avail;
int fd[2];
char buf[256];

void *fun1(void *arg)
{
	close(fd[0]);
	while (1)
	{
		sem_p(mutex);
		sem_p(avail);
		sem_v(full);
		
	}
	
	close(fd[1]);
}

void *fun2(void *arg)
{
	close(fd[1]);
	while (1)
	{
		sem_v(mutex);
		sem_p(full);
		sem_v(avail);
	}
	
	close(fd[0]);
}

int main()
{	
	if (mkfifo(".",0666) != 0)
	{
		perror("mkfifo err");
	}
	
	pthread_t tid1, tid2;
	
	full = init_sem(N,0);
	mutex = init_sem(1,1);
	avail = init_sem(N,N);
	
	if (pthread_create(&tid1,NULL,fun1,NULL) < 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if (pthread_create(&tid2,NULL,fun2,NULL) < 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
}
