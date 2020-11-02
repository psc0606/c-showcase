#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int fd1;
static int fd2;

int init_pipe()
{
	if (pipe(fd1) < 0)
	{
		perror("pipe fd1 err");
		exit(0);
	}
	if (pipe(fd2) < 0)
	{
		perror("pipe fd2 err");
		exit(0);
	}
	return 0;
}

int wait_father()
{
	int n;
	char ch;
	if ((n = read(fd2[0],&ch,1)) != 1)
	{
		perror("wait father err");
		return -1;
	}
	return 0;
}

int wait_child()
{
	int n;
	char ch;
	if ((n = read(fd1[0],&ch,1)) != 1)
	{
		perror("wait chlid err");
		return -1;
	}
	return 0;
}

int tell_child()
{
	char ch = 'f';
	if (write(fd1[1],&ch,1) != 1)
	{
		perror("tell child err");
		return -1;
	}
	return 0;
}

int tell_father()
{
	char ch = 'c';
	if (write(fd2[1],&ch,1) != 1)
	{
		perror("tell father err");
		return -1;
	}
	return 0;
}
