#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int fd;
	char buf[1024] = {0};
	if ((fd = open("1.txt",O_RDONLY)) < 0)
	{
		perror("open fd failed!\n");
		exit(0);
	}
	if (read(fd, buf, sizeof(buf)) < 0)
	{
		perror("read failed!\n");
		exit(0);
	}
	if (write(1, buf, strlen(buf)) < 0)
	{
		perror("write failed!\n");
		exit(0);
	}
	close(fd);
	return 0;
}
