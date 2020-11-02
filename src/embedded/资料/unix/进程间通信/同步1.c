#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define FIFO "/tmp/fifo.1"

int main(int argc, char **argv)
{
	int fd;
	char w_buf[8192];
	int real_num;
	memset(w_buf,0,8192);
	if ((mkfifo(FIFO,0644) < 0) && (errno != EEXIST))
	{
		printf("cannot creat fifo");
	}
	fd = open(FIFO,O_WRONLY | O_NONBLOCK,0);
	if (fd < 0)
	{
		perror("open fifo err");
		return -1;
	}
	real_num = write(fd,w_buf,5000);
	if (real_num == -1)
	{
		perror("write err");
	}
	else
	{
		printf("real write num is %d\n",real_num);
	}
}
