#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i = 3;
	FILE *fp;
	while (1)
	{
		fp = fopen("./file3.txt","w");
		if (fp != NULL)
		{
			fprintf(stdout,"open sucessfully\n");
			i++;
			//fclose(fp);
		}
		else
		{
			fprintf(stderr,"open failure\n");
			break;
		}
	}
	printf("%d",i);
	return 0;
}
