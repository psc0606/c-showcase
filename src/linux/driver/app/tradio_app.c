#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	char buf[4] = {0};
	int fd;
	int n;

	fd = open("/dev/tradio1", O_RDWR);

	lseek(fd, 0x80c0, SEEK_SET);
	memset(buf, 0x33, sizeof(buf));
	n = write(fd, buf, sizeof(buf));
	printf("%d\n", n);
	n = 0;

	memset(buf, 0, sizeof(buf));
	lseek(fd, 0x80c0, SEEK_SET);
	if(n = read(fd, buf, sizeof(buf)) == -1) {
		perror("read");
		return -1;
	}
	printf("val:0x%08x\n", *(unsigned long int *)buf);
	printf("%d\n", n);
//	printf("val:0x%x %x %x %x\n", buf[0], buf[1],buf[2],buf[3]);
	close(fd);
	return 0;
}
