#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd;
	char buf[1024] = {0};
	if ((fd = open("1.txt", O_CREAT | O_RDWR, 0666)) < 0)
	{
		perror("open fd failed!\n");
		exit(0);
	}
	int flag = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, flag | O_APPEND);
	if (read(STDIN_FILENO, buf, sizeof(buf)) < 0)
	{
		perror("read failed!\n");
		exit(0);
	}
	if (write(fd, buf, strlen(buf)) < 0)
	{
		perror("write failed!\n");
		exit(0);
	}
	if (close(fd) < 0)
	{
		perror(NULL);
	}
	return 0;
}
