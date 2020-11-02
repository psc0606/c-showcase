#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd1, fd2;
	if ((fd1 = open("1.txt", O_CREAT | O_RDWR, 0666)) < 0)
	{
		perror("open fd1 failed!\n");
		exit(0);
	}
	if ((fd2 = open("2.txt", O_CREAT | O_RDWR, 0666)) < 0)
	{
		perror("open fd2 failed!\n");
		exit(0);
	}
	fprintf(stdout, "fd1:%d,fd2:%d\n",fd1,fd2);
	close(fd1);
	close(fd2);
	fprintf(stdout, "fd1:%d,fd2:%d\n",fd1,fd2);
}
