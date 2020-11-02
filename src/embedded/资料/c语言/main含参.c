#include <stdio.h>
int main(int argc,char **argv)
{
	/*while(*++argv != NULL)
	{
		printf("%s\n",*argv);//*(argv++)
	}
	return 1;
	*/
	int i;
	for (i = 0; i < argc; i++)
	{
		printf("%s\n",*(argv+i));
	}
}
