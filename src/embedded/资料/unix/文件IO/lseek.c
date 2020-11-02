#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd;
	char buf[1024] = {0};
	if ((fd = open("1.txt",O_CREAT | O_RDWR, 0666)) < 0)
	{
		perror("open fd failed!\n");
		exit(0);
	}
	if (lseek(fd,3,SEEK_SET) < 0)
	{
		perror("lseek failed!\n");
		exit(0);
	}
	if (read(fd, buf, sizeof(buf)) < 0)
	{
		perror("read failed!\n");
		exit(0);
	}
	if (write(STDOUT_FILENO, buf, strlen(buf)) < 0)
	{
		perror("write failed!\n");
		exit(0);
	}
	if (close(fd) < 0)
	{
		perror(NULL);
	}
}
