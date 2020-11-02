#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXN_ITEMS 1000000
#define MAXN_THREADS 100
int nitems;

struct
{
	pthread_mutex_t mutex;
	int buf[MAXN_ITEMS];
	int nput;
	int nval;
}share = {PTHREAD_MUTEX_INITIALIZER};

int min(int a,int b)
{
	return a > b ? b:a;
}

void *consume(void *arg)
{
	int i;
	for (i = 0; i < nitems; i++)
	{
		if (share.buf[i] != i)
		{
			printf("buf[%d] = %d\n",i,share.buf[i]);
		}
	}
	return NULL;
}

void *produce(void *arg)
{
	for (;;)
	{
		pthread_mutex_lock(&share.mutex);
		if (share.nput >= nitems)
		{
			//printf("nputs :%d\n",share.nput);
			pthread_mutex_unlock(&share.mutex);
			return NULL;
		}
		share.buf[share.nput] = share.nval;
		share.nput++;
		share.nval++;
		pthread_mutex_unlock(&share.mutex);
		*(int*)arg += 1;
	}
}

int main(int argc,char **argv)
{
	int i,nthreads,count[MAXN_THREADS];
	pthread_t tid_produce[MAXN_THREADS],tid_consume;
	if (argc != 3)
	{
		fprintf(stderr,"format err:\n");
		exit(0);
	}
	nitems = min(atoi(argv[1]),MAXN_ITEMS);
	nthreads = min(atoi(argv[2]),MAXN_THREADS);
	printf("nitems:%d,nthread:%d\n",nitems,nthreads);
	for (i = 0; i < nthreads; i++)
	{
		count[i] = 0;
		pthread_create(&tid_produce[i],NULL,produce,&count[i]);
	}
	for (i = 0; i < nthreads; i++)
	{
		pthread_join(tid_produce[i],NULL);
		printf("count[%d] = %d\n",i,count[i]);
	}
	pthread_create(&tid_consume,NULL,consume,NULL);
	pthread_join(tid_consume,NULL);
}
