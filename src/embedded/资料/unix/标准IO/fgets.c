#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	if ((fp = fopen("1.txt","r+")) == NULL)
	{
		perror("fopen err");
		exit(0);
	}
	char buf[1024];
	fgets(buf,1024,fp);
	printf("%s",buf);
	//fclose(fp);
}
