#include <stdio.h>
#include <stdlib.h>

void fun()
{
	printf("fun\n");
}

void test()
{
	pid_t pid;
	pid = vfork();
	if (pid == 0)
	{
		printf("child\n");
		//return;
		exit(0);
	}
	else
	{
		printf("father\n");
	}
}

int main()
{
	test();
	fun();
	printf("main\n");
}
