#include <stdio.h>
#include <pthread.h>

int main()
{
	printf("main tid:%d0x(%x)\n",pthread_self(),pthread_self());
}
