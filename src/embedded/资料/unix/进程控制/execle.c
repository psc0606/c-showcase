#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char *envp[] = {"PATH = /bin","USER = root","LANG=zh_CN.utf8",NULL};
	if (execle("/bin/ls","ls","-l",NULL,envp) < 0)
	{
		perror("execle failed!\n");
		exit(0);
	}
}
