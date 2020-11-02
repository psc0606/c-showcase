#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE *fp;
	if ((fp = fopen("1.txt","r")) == NULL)
	{
		perror("open error");
		exit(0);
	}
	
	fseek(fp,0,SEEK_END);

	printf("%ld\n",ftell(fp));
	fclose(fp);
	return 0;
}
