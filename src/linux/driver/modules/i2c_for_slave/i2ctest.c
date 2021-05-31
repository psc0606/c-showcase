#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h> //ioctl使用的命令
#include <unistd.h>

//#define DEVICE_NAME	"/dev/i2c-0"
//#define SLAVE_DEVICE_ADDR 0x10 //挂在i2c总线上从设备地址

#define CON_PROMOT() printf("$>>>>")

struct i2c_rdwr_ioctl_data data; //i2c-dev.h定义

int open_device()
{
	int fd;
	
	fd = open("/dev/i2c-0", O_RDWR);
	if(fd == -1) { //no /dev/i2c-0 exit
		printf("Fail to open /dev/i2c-0\n");
		return -1;
	}
	return fd;
}

void close_device(int fd)
{
	close(fd);
	return;
}

#define MAX_PARAM 4
int main()
{
	int fd, i;
	char *para, cmd[20] = {0};
	char ucpara[MAX_PARAM] = {0};
	char buf[3];

	printf("format:<r/w/q> <addr> <reg> r:read, w:write, q:quit\n");
	while(1) {
		CON_PROMOT();
		fgets(cmd, sizeof(cmd), stdin);
				
		para = strtok(cmd, " ");
		if(!para)
			continue;
		ucpara[0] = *para;

		int i = 1;
		while(1) {
			para = strtok(NULL, " ");
			if(para == NULL)
				break;
			ucpara[i++] = strtoul(para, NULL, 16);
			if(i > 4)	
				break;
		}
		memset(buf, ucpara[1], 1); //device addr
		strncpy(buf+1, ucpara+2,2); //reg addr and data

//	printf("0x%x,0x%x,0x%x\n", buf[0], buf[1], buf[2]);
		switch(ucpara[0]) {
			case 'r':
				fd = open_device();
				read(fd, buf, sizeof(buf));
				printf("read:0x%x\n", buf[2]);
				close_device(fd);
				break;
			case 'w':
				fd = open_device();
				write(fd, buf, sizeof(buf));
				close_device(fd);
				break;
			case 'q':
				return;
			case '\n':
				continue;		
			default:
				printf("Invaild command\n");
		}
	}
	
//	ioctl(fd, I2C_TIMEOUT, 2); //设置超时时间,默认为1
//	ioctl(fd, I2C_RETRIES, 1); //设置收不到ACK时重发次数
	
//	write_data(fd, 0x0b, 0x45);
//	read_data(fd, 0x05);
	
	return 0;
}
