#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd, n = 0;
	while (1)
	{
		if ((fd = open("1.txt",O_CREAT | O_TRUNC, 0666)) < 0)
		{
			perror("fd open failed!\n");
			break;
		}
		n++;
	}
	printf("%d\n",n);
	close(fd);
	return 0;
}
