#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "spi.h"
#define SPI_DEV "/dev/spidev0.1"

int main(int args, char *argv[])
{
	int fd;
	int addr;
	int rbuf, wbuf;

	fd = open(SPI_DEV, O_RDWR);
	if(fd < 0) {
		perror("can not open spidev");
	}

	SpiDrvWriteReg(fd, 0x03, 1);
	addr = strtol(argv[1], NULL, 16);
	printf("addr:0x%x\n", addr);
	rbuf = 0xff;
//	rbuf = SpiDrvReadReg(fd, addr);
	printf("rbuf:0x%x\n", rbuf);

	close (fd);
	return 0;
}
