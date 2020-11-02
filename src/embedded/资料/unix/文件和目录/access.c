#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv)
{
	if (access(argv[1],F_OK) != 0)
	{
		printf("no exit");
		exit(0);
	}
	if (access(argv[1],R_OK) == 0)
	{
		printf("r");
	}
	if (access(argv[1],W_OK) == 0)
	{
		printf("w");
	}
	if (access(argv[1],X_OK) == 0)
	{
		printf("x");
	}
}
