#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd;
	FILE *fp;
	char buf[1024] = {0};
	fp = fopen("1.txt","r+");
	if (fp == NULL)
	{
		perror("fopen failde!\n");
		exit(0);
	}
	fd = fileno(fp);
	if (read(fd, buf, sizeof(buf)) < 0)
	{
		perror("read failed!\n");
		exit(0);
	}
	printf("%s",buf);
	if (close(fd) < 0)
	{
		perror(NULL);
	}
	return 0;
}
