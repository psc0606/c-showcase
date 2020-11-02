#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	//if (link(argv[1],argv[2]) < 0)
	if (symlink(argv[1],argv[2]) < 0)
	{
		perror("link err");
		exit(0);
	}
	//readlink();读出来是文件名
}
