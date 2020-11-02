#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

static int fd1[2];
static int fd2[2];

int init_pipe()
{
	if(pipe(fd1) < 0)
	{
		perror("pipe fd1 err");
		return -1;
	}
	if(pipe(fd2) < 0)
	{
		perror("pipe fd2 err");
		return -1;
	}
	return 0;
}

int wait_father()
{
	char ch;
	if(read(fd1[0],&ch,1) != 1)
	{
		perror("wait father err.");
		return -1;
	}
	return 0;
}

int wait_child()
{
	char ch;
	if(read( fd2[0],&ch,1 ) != 1)
	{
		perror(" wait child err.");
		return -1;
	}
	return 0;
}

int tell_child()
{
	char ch = 'f';
	if(write(fd1[1],&ch,1) != 1)
	{
		perror("tell child err.");
		return -1;
	}
	return 0;
}

int tell_father()
{
	char ch = 'c';
	if(write(fd2[1],&ch,1) != 1)
	{
		perror("tell father err.");
		return -1;
	}
	return 0;
}

