#include <stdio.h>
#include <pthread.h>

void printid(char *ptr)
{
	printf("%s\tpid:%d\ttid:0x%x\n",ptr,getpid(),pthread_self());
}

void *fun(void *arg)
{
	printf("arg:%s\n",arg);
	printid("fun1:");
	return (void *)0;
}

int main()
{
	pthread_t tid;
	char *s = "adfqwer";
	if (pthread_create(&tid,NULL,fun,s) != 0)
	{
		perror("creat pthread er");
	}
	pthread_cancel(tid);
	printid("main thread:\n");
	sleep(1);
}
