#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char *argv[] = {"ls","-l",NULL};
	if (execvp("ls",argv) < 0)
	{
		perror("execvp failed!\n");
	}
}
