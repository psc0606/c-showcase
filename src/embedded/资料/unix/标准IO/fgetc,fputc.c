#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	int a;
	FILE *fp1, *fp2;
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if ((fp1 = fopen(argv[1],"r+")) == NULL)
	{
		perror("fp1 open failed.");
		exit(0);
	}
	if ((fp2 = fopen(argv[2],"w")) == NULL)
	{
		perror("fp2 open failed.");
		exit(0);
	}
	while (1)
	{
		a = fgetc(fp1);
		if (feof(fp1))
		{
			break;
		}
		fputc(a,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
