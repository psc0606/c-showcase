#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main()
{
	FILE *fp;
	fp = fopen("./file3.txt","r");
	if (fp == NULL)
	{
		//perror(NULL);
		printf("%s\n",strerror(errno));
		exit(0);
	}
	fclose(fp);
	return 0;
}
