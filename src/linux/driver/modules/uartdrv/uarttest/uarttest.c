/********************************************************************************
 *       uarttest.c                                                              *
 *                                                                              *
 *******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "configure.h"

#define UART_DEV_NAME    "/dev/uartdrv"
#define RECEIVE_LEN  500

int length_str(char *str)
{
    int n = 0;
    char *p = str;
    while(*p != '\xFE'){
	++p;
	++n;
    }
    return n;
}

void gps_config(int fd)
{
	int i;
	int res;
	unsigned int write_length;

	char temp = 0xB5;
	printf("config GPS...\n");
//	write(fd, &temp, 1);
	
	for(i = 0; i < 10; i++)
	{
	    write_length = length_str(buf[i]); 
    	printf("write length is %d\n", write_length);

	    if((res = write(fd, buf[i], write_length)) == -1)
		{
			printf("config GPS failure.\n");
			return ;
	    }
	}
	
	printf("config GPS done.\n");
}

void gps_receive(int fd)
{
	int i = 0;
	int status = -1;
	printf("GPS Receiving...\n");
	char *data = (char *)malloc(sizeof(char)*RECEIVE_LEN);
	memset(data, 0, sizeof(char)*RECEIVE_LEN);
	if((status = read(fd, data,RECEIVE_LEN)) == -1)
	{
		perror("Read failed\n"); 
		return;
	}

	for(i = 0; i < RECEIVE_LEN; i++)
	{
//		printf("%d", data[i]); 
		printf("%c", toascii(data[i]));
	}
	putchar('\n');
}

int main()
{
	int fd = -1;

	fd = open(UART_DEV_NAME, O_RDWR | O_SYNC);
	if(fd < 0){
		perror("open uart dev error\n");
		return -1;
	}
	gps_config(fd);

	gps_receive(fd);

	close(fd);
	return 0;
}
