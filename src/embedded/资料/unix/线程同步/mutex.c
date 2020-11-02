#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *fun1(void *arg)
{
	int i = 0;
	char *a = (char*)arg;
	printf("%s start.\n",a);
	//pthread_mutex_lock(&mutex);
	while (i++ < 5)
	{
		printf("%s hello world.\n",a);
		//sleep(1);
	}
	//pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_t tid1, tid2;
	
	if (pthread_create(&tid1,NULL,fun1,"tid1") != 0)
	{
		perror("create err");
	}
	if (pthread_create(&tid2,NULL,fun1,"tid2") != 0)
	{
		perror("create err");
	}
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
}
