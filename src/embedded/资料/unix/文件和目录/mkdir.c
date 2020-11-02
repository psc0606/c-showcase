#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	if (mkdir(argv[1],0777) < 0)
	{
		perror("error");
		exit(0);
	}
	//rm();
}
