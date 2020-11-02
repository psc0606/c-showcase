#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd;
	//fd = open("1.txt",O_WRONLY);
	//fd = open("2.txt",O_WRONLY | O_CREAT,0777);//不能设置，umask	0000
	fd = open("2.txt",O_WRONLY | O_CREAT | O_EXCL,0777);
	if (fd < 0)
	{
		perror("open failed\n");
	}
	return 0;
}
