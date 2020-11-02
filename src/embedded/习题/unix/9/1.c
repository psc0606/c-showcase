/*----------------------------------------
**	1. 	主线程产生两个线程，两个线程分别每
**	隔1s和5s向屏幕输出信息，主线程等待用
**	户按下任意键，然后使两个子线程退出。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *fun1(void *arg)
{
	while (1)
	{
		printf("fun 1 is running!\n");
		sleep(1);
	}
}

void *fun2(void *arg)
{
	while (1)
	{
		sleep(5);
		printf("fun 2 is running!\n");
	}
}

int main()
{
	pthread_t tid1, tid2;
	void *tret;
	char a;
	if ((pthread_create(&tid1,NULL,fun1,NULL)) != 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if ((pthread_create(&tid2,NULL,fun2,NULL)) != 0)
	{
		perror("create pthread 2 err");
		exit(0);
	}
	
	if ((a = getchar()) != EOF)
	{
		pthread_cancel(tid1);
		pthread_cancel(tid2);
	}
}
