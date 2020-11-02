#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	if (execlp("ls","ls","-l",NULL) < 0)
	{
		perror("execlp failed!\n");
	}
}
