#include <stdio.h>
#include <pthread.h>

void printid(char *ptr)
{
	printf("%s\tpid:%d\ttid:0x%x\n",ptr,getpid(),pthread_self());
}

void *fun(void *arg)
{
	//printf("arg:%d\n",*(int*)arg);
	printf("arg:%s\n",arg);
	pthread_exit((void*)5);
	printid("fun1:");
	return (void *)0;
}

int main()
{
	pthread_t tid;
	int a = 10;
	char *s = "adfqwer";
	if (pthread_create(&tid,NULL,fun,s) != 0)
	{
		perror("creat pthread er");
	}
	//pthread_exit((void*)5);主线程退出，其他线程继续
	printid("main thread:\n");
	sleep(1);
}
