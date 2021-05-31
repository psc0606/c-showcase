#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h> //ioctl使用的命令
#include <unistd.h>

//#define DEVICE_NAME	"/dev/i2c-0"
#define SLAVE_DEVICE_ADDR 0x20 //挂在i2c总线上从设备地址

#define CON_PROMOT() printf("$>>>>")

struct i2c_rdwr_ioctl_data data; //i2c-dev.h定义

int open_device()
{
	int fd;
	
	fd = open("/dev/i2c-0", O_RDWR);
	if(fd == -1) { //no /dev/i2c-0 exit
		printf("Fail to open /dev/i2c-0\n");
		printf("Try to open /dev/i2c-1\n");
		fd = open("/dev/i2c-1", O_RDWR);
		if(fd == -1) {
			printf("No /dev/i2c-* exit\n");
			return -1;
		}
	}
	return fd;
}

unsigned char read_data(int fd, unsigned char addr)
{
	char buf = 0;
	data.nmsgs = 2;  //nmsgs中决定启始信号个数
	data.msgs = (struct i2c_msg*)malloc(sizeof(struct i2c_msg) * data.nmsgs);
	if(!data.msgs) {
		printf("malloc err\n");
		close(fd);
		exit(0);
	}
	
	/* read data from i2c slave device */
	(data.msgs[0]).len = 1;//1 byes= 8bit
	(data.msgs[0]).addr = SLAVE_DEVICE_ADDR;//从设备地址
	(data.msgs[0]).flags = 0;     //写
	
	(data.msgs[0]).buf = &addr; //取代以下两行
	//(data.msgs[0]).buf = (unsigned char*)malloc(sizeof(unsigned char)); //没有，会出现段错误
	//(data.msgs[0]).buf[0] = addr; //寄存器地址,buf是一个指针，取的是指针的值, 用下面的语句也可以
//	*(data.msgs[0]).buf = 0x03;

	(data.msgs[1]).len = 1;
	(data.msgs[1]).addr = SLAVE_DEVICE_ADDR;//从设备地址
	(data.msgs[1]).flags = I2C_M_RD;     //读, i2c.h
	
	(data.msgs[1]).buf = &buf;
	//(data.msgs[1]).buf = (unsigned char*)malloc(sizeof(unsigned char));
	//(data.msgs[1]).buf[0] = 0; //初始化,读到结果会放在此处

	if(ioctl(fd, I2C_RDWR, (unsigned long)&data) == -1) {
		printf("ioctl:fail to read\n");
		close(fd);
		exit(0);
	}
	printf("read 0x%x from 0x%x\n", buf, addr);
	free(data.msgs);
	return buf;
	
	//printf("read 0x%x from 0x%x\n", data.msgs[1].buf[0], addr);
	//free((data.msgs[0]).buf); //malloc,记得free
	//free((data.msgs[1]).buf);
	//return data.msgs[1].buf[0];
}

void write_data(int fd, unsigned char addr, unsigned char buf)
{	/* write data to i2c slave device */
#if 0   
	/* 用write的方法实现 */
	int err = ioctl(fd, I2C_SLAVE, SLAVE_DEVICE_ADDR);
	if(err < 0) {
		printf("ioctl failed:%d\n", err);
		return;
	}
	err = write(fd, buf, 2);
	if(err < 0) {
		printf("write failed:%d\n", err);
		return;
	}
	printf("write %d\n", err);
#endif
	
#if 1
	data.nmsgs = 1;//写时只有一次时序,设置错误会导致ioctl调用失败
	data.msgs = (struct i2c_msg*)malloc(sizeof(struct i2c_msg) * data.nmsgs);
	if(!data.msgs) {
		printf("malloc err\n");
		close(fd);
		exit(0);
	}
	//(data.msgs)->len = 2; //1 byes= 8bit, 看时序。或下面的写法
	(data.msgs[0]).len = 2; //1 byes= 8bit
	(data.msgs[0]).addr = SLAVE_DEVICE_ADDR;//从设备地址
	(data.msgs[0]).flags = 0; //写
	//(data.msgs[0]).buf = buf; //替代下面3行
	
	(data.msgs[0]).buf = (unsigned char*)malloc(2*sizeof(unsigned char)); //没有，出现段错误
	(data.msgs[0]).buf[0] = addr; //寄存器地址,buf是一个指针，取的是指针的值, 用下面的语句也可以
	(data.msgs[0]).buf[1] = buf;  //写的内容

	if(ioctl(fd, I2C_RDWR, (unsigned long)&data) == -1) {
		printf("ioctl:fail to write\n");
		close(fd);
		exit(0);
	}
	printf("write 0x%x to 0x%x\n", (data.msgs[0]).buf[1], (data.msgs[0]).buf[0]);
	free((data.msgs[0]).buf);
	free(data.msgs);
#endif
}

void close_device(int fd)
{
	close(fd);
	return;
}

typedef unsigned long u64;
#define MAX_PARAM 3
int main()
{
	int fd, i;
	char *para, cmd[20] = {0};
	u64 ucpara[MAX_PARAM] = {0};

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
			if(i > 2)	
				break;
		}

//	printf("%c %u %u\n", ucpara[0], ucpara[1], ucpara[2]);
		switch(ucpara[0]) {
			case 'r':
				fd = open_device();
				read_data(fd, ucpara[1]);
				close_device(fd);
				break;
			case 'w':
				fd = open_device();
				write_data(fd, ucpara[1], ucpara[2]);
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
