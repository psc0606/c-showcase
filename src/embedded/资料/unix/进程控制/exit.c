#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		printf("please input the arguments\n");
		exit(0);
	}
	else if ((strcmp(argv[1],"1")) == 0)
	{
		printf("using exit..\n");
		printf("this is the content in buffer");
		exit(0);
	}
	else if (strcmp(argv[1],"2") == 0)
	{
		printf("using exit...\n");
		printf("this is the content in buffer");
		_exit(0);
	}
	else
	{
		printf("wrong argument");
	}
	return 0;
}
