#include <stdio.h>
#include <stdlib.h>

void main()
{
	char buf[3];
	setvbuf(stdout,buf,_IOLBF,sizeof(buf));
	printf("123");
	while (1);
}
