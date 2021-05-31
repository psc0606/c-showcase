#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include "../spi.h"

unsigned short str_to_hex(char *str)
{
    short i, j;
    unsigned short tmp,num_hex,hex_mod;
    unsigned short len = strlen(str);
    if (len < 3 || len > 5)
	return 0xffff;
    num_hex = 0;
    hex_mod = 1;
    for (i = len - 1, j = 0; j < len - 2; i--, j++)
    {
	if ((str[i] >= '0' && str[i] <= '9'))
	    tmp = str[i] - '0';
	else if ((str[i] >= 'a' && str[i] <= 'f'))
	    tmp = str[i] - 'a' + 10;
	else if ((str[i] >= 'A' && str[i] <= 'F'))
	    tmp = str[i] - 'A' + 10;
	else 
	    return 0xffff;
	num_hex += tmp * hex_mod;
	hex_mod *= 16;
    }
    if ('x' != str[i])
	return 0xffff;
    return num_hex;
}
int main(int argc, char* argv[])
{
    int fd;
    unsigned int val = 0;
    unsigned short addr, data;
    char help[] = { "Input form ERROR!!!\n"
		    "Example:\n"
		    "w 0x250 0xff\n"
		    "r 0x251\n"
		   };
    char option0[] = "r";
    char option1[] = "w";
    fd = open("/dev/fpga", O_RDWR);
    if(fd < 0)
    {
        printf("Cannot open /dev/fpga\n");
        return -1;
    }
    if (3 == argc && 0 == strcmp(option0, argv[1]))
    {
	printf("Reading Ad9361Reg ...\n");
	addr = str_to_hex(argv[2]);
	if (addr >= 0x3ff)
	{
	    printf("Addr. Error!\n");
	    printf("%s\n", help);
	    return 1;
	}
	printf("Val in 0x%x is 0x%x\n", addr, read_spi(fd, addr));
	printf("Read Done!\n");
    }
    else if (4 == argc && 0 == strcmp(option1, argv[1]))
    {
	printf("Writing Ad9361Reg ...\n");
	data = str_to_hex(argv[3]);
	addr = str_to_hex(argv[2]);
	if (addr >= 0x3ff)
	{
	    printf("Addr. Error!\n");
	    printf("%s\n", help);
	    return 1;
	}
	write_spi(fd, addr, data);
	printf("Write the val 0x%x to Addr. 0x%x\n", data, addr);
	printf("Write Done!\n");
    }
    else
	printf("%s\n", help);
    close(fd);
    return 0;
}
