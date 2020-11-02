#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <mtd/mtd-abi.h>
#define DEV_NAME "/dev/mtd2"

int main()
{
	int fd, ofd;
	struct mtd_info_user meminfo;
	char buf[4096] = {0};

	fd = open(DEV_NAME, O_RDWR);
	if(fd < 0) {
		perror("open Flash");
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &meminfo) != 0) {
		perror("MEMGETINFO");
		close(fd);
		exit(1);
	}

	ofd = open("./output", O_RDWR | O_CREAT);
	if(ofd < 0) {
		perror("open output file");
		exit(1);
	}

	memset(buf, 'e', sizeof(buf));
	write(fd, buf, sizeof(buf));

	lseek(fd, 0, SEEK_SET);
	
	memset(buf, '\0', sizeof(buf));
	pread(fd, buf, sizeof(buf), 0);
	write(ofd, buf, sizeof(buf));

	close(fd);
	close(ofd);

	return 0;
}
