#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#define CS1_BASE (0xa08000000)
#define CS0_BASE (0xa0000000)
#define CS1_SIZE (0x1000)

/*
unsigned short str_to_hex(char *str)
{
    short i, j;
    unsigned short tmp,num_hex,hex_mod;
    unsigned short len = strlen(str);
    num_hex = 0;
    hex_mod = 1;
    if(len >= 3)
    for (i = len - 1, j = 0; j < len - 2; i--, j++)
    {
//	printf("1\n");
	if (len < 3 || len > 5)
	    return 0xffff;
//	printf("2\n");
	if ((str[i] >= '0' && str[i] <= '9'))
	    tmp = str[i] - '0';
	else if ((str[i] >= 'a' && str[i] <= 'f'))
	    tmp = str[i] - 'a' + 10;
	else if ((str[i] >= 'A' && str[i] <= 'F'))
	    tmp = str[i] - 'A' + 10;
	else 
	    return 0xffff;
//	printf("3\n");
	num_hex += tmp * hex_mod;
	hex_mod *= 16;
//	if ('x' != str[i])
//	    return 0xffff;
    }
    else
    for (i = len - 1, j = 0; j < len; i--, j++)
    {
	if ((str[i] >= '0' && str[i] <= '9'))
	    tmp = str[i] - '0';
	else 
	    return 0xffff;
	num_hex += tmp * hex_mod;
	hex_mod *= 10;
    }
    return num_hex;
}
*/

int main(int argc, char *argv[])
{
    int size = 1;
    int fd_mem;
    unsigned char data;
    unsigned short addr;
    char help[] = { "Input form ERROR!!!\n"                                                                   
		    "RWfgpa r/w addr [val]\n"
                    "Example:\n"
                    "w 0x250 0xff\n"
                    "r 0x250\n"
                  };
    char option0[] = "r";
    char option1[] = "w";
    fd_mem = open("/dev/fpga", O_RDWR);
    if(fd_mem < 0) {
	printf("Cannot Open /dev/fpga.\n");
	return -1;
    }
    if (3 == argc && 0 == strcmp(option0, argv[1]))
    {
	printf("Reading FPGA ...\n");
	addr = str_to_hex(argv[2]);
	if (addr >= 0x1000)
	{
	    printf("Read Addr. Error!\n");
	    printf("%s\n", help);
	    return 1;
	}
	lseek(fd_mem, addr, SEEK_SET);
	read(fd_mem, &data, size);
	printf("Val in 0x%x is 0x%x\n", addr, data);
	printf("Read Done!\n");
    }
    else if (4 == argc && 0 == strcmp(option1, argv[1]))
    {
	printf("Writing FPGA ...\n");
	data = str_to_hex(argv[3]);
	addr = str_to_hex(argv[2]);
	if (addr >= 0x1000)
	{
	    printf("Addr. Error!\n");
	    printf("%s\n", help);
	    return 1;
	}
	lseek(fd_mem, addr, SEEK_SET);
	write(fd_mem, &data, size);
	printf("Write the val 0x%x to Addr. 0x%x\n", data, addr);
	printf("Write Done!\n");
    }
    else
	printf("%s\n", help);
    close(fd_mem);
    return 0;
}
