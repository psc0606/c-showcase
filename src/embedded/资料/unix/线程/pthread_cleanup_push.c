#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void cleanup(void *arg)
{
	printf("cleanup:%s\n",(char*)arg);
}

void *fun1(void *arg)
{
	printf("thread 1 start\n");
	pthread_cleanup_push(cleanup,"thread1 first handler");
	pthread_cleanup_push(cleanup,"thread1 second handler");
	printf("thread1 push complete\n");
	if (arg != NULL)
	{
		return (void*)1;
	}
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	return (void*)1;
}

int main()
{
	pthread_t tid1, tid2;
	void *tret;
	if (pthread_create(&tid1,NULL,fun1,(void*)0) != 0)
	{
		perror("create fun 1 err");
		exit(0);
	}
	pthread_join(tid1,&tret);
	printf("tid1 exit code:%d\n",(int)tret);
}
