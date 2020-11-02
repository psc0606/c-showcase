#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char **argv)
{
	umask(0000);
	if (open(argv[1],O_CREAT | O_RDWR,0666) < 0)
	{
		perror("open failed!\n");
		return -1;
	}
}
