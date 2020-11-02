#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	if ((fp = fopen("1.txt","w+")) == NULL)
	{
		printf("fopen failed!\n");
		return 0;
	}
	fprintf(fp,"hello world!");
	printf("read:%s\n",fp->_IO_read_ptr);
	printf("write:%s",fp->_IO_write_ptr);
	return 1;
}
