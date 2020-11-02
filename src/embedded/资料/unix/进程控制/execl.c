#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
int main(int argc,char **argv, char **env)
{
	while (*env != NULL)
	{
		printf("%s\n",*env);
		env++;
	}
}
*/
int main()
{
	if (execl("/bin/ls","ls","-l",NULL) < 0)
	{
		perror("execl failed!\n");
	}
}
