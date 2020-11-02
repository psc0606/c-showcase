#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *thr_fun1(void *arg)
{
	printf("thread 1 exit.\n");
	pthread_exit((void*)1);
}

void *thr_fun2(void *arg)
{
	printf("thread 2 exit.\n");
	pthread_exit((void*)2);
}

int main()
{
	int err;
	pthread_t tid1, tid2;
	void *tret;
	if ((err = pthread_create(&tid1,NULL,thr_fun1,NULL)) != 0)
	{
		perror("create thread 1 err");
		exit(1);
	}
	pthread_join(tid1,&tret);
	printf("tid1:%d\n",tret);
	if ((err = pthread_create(&tid2,NULL,thr_fun2,NULL)) != 0)
	{
		perror("create thread 2 err");
		exit(1);
	}
	pthread_join(tid2,&tret);
	printf("tid2:%d\n",tret);
	printf("%d\n",pthread_equal(tid1,tid2));
	exit(0);
}
