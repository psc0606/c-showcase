#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define FIFO "/tmp/fifo.1"

int main()
{
	char r_buf[8192];
	int fd;
	int ret_size;
	memset(r_buf,0,sizeof(r_buf));
	//fd = open(FIFO,O_RDONLY | O_NONBLOCK);
	fd = open(FIFO,O_RDONLY);
	if (fd < 0)
	{
		perror("open fd err");
		return -1;
	}
	while (1)
	{
		memset(r_buf,0,sizeof(r_buf));
		ret_size = read(fd,r_buf,4098);
		if (ret_size < 0)
		{
			perror("read err");
		}
		printf("real read bytes %d \n",ret_size);
		sleep(1);
	}
	pause();
	unlink(FIFO);
}
