#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct stat buf;
	int fd;
	if (argc != 2)
	{
		perror("format err");
		exit(0);
	}	
	if ((fd = open(argv[1],O_CREAT|O_RDWR,0666)) < 0)
	{
		perror("open err");
		exit(0);
	}
	if (stat(argv[1],&buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		perror("fork err");
		exit(0);
	}
	else if(pid == 0)
	{
		char buf[] = "1. ";
		if(write(fd,&buf,strlen(buf)) != strlen(buf))
		{
			perror("write err");	
		}
		exit(0);
	}
	else
	{
		time_t tm;
		char buf[128];
		time(&tm);
		snprintf(buf,sizeof(buf),"%s",ctime(&tm));
		wait(NULL);
		if( write(fd,buf,strlen(buf)) != strlen(buf))
		{
			perror("write err");
		}
	}
}
