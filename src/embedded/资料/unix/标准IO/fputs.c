#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	char buf[1024],*a;
	FILE *fp1, *fp2;
	if (argc != 3)
	{
		perror("error");
		exit(0);
	}
	if ((fp1 = fopen(argv[1],"r")) == NULL)
	{
		perror("error");
		exit(0);
	}
	if ((fp2 = fopen(argv[2],"w")) == NULL)
	{
		perror("error");
		exit(0);
	}
	
	while (1)
	{
		a = fgets(buf,sizeof(buf),fp1);
		if (feof(fp1))
		{
			break;
		}
		fputs(a,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
