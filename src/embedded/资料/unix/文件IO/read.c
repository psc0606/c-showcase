#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char buf[1024];
	read(0, buf, sizeof(buf));
	printf("%s",buf);
	return 0;
}
