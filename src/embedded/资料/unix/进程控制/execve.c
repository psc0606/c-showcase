#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char *envp[] = {NULL};
	char *argv[] = {"ls","-l",NULL};
	if (execve("/bin/ls",argv,envp) < 0)
	{
		perror("execve failed!\n");
		exit(0);
	}
}
