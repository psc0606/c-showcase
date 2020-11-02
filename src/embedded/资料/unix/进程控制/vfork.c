#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	int count = 1;
	pid = vfork();
//	count++;
	if (pid == 0)
	{
		count++;
		printf("count: %d\n",count);
		exit(0);
	}
	else if (pid > 0)
	{
		printf("count:%d\n",count);
	}
}
