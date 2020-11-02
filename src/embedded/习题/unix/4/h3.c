/*----------------------------------------
**	3． 写一个程序，当程序运行时，可以输入shell
**	命令并执行，就像在终端里一样。如下图所示：并且
**	输入”quit”可以退出程序。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void fun(char *argv[6])
{
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		perror("fork err");
		exit(0);
	}
	else if (pid == 0)
	{
		if (execvp(argv[0],argv) < 0)
		{
			perror("execvp failed!\n");
		}
		exit(0);
	}
	else
	{
		wait(NULL);
	}
}

int main()
{
	int i, j;
	while (1)
	{
		int i;
		char *argv[6] = {NULL};
		char buf[128] = {0},src[128] = {0};
		getcwd(src,sizeof(src));
		for (i = 0; i < strlen(src); i++)
		{
			if (src[i] == '/')
			{
				memmove(src,src+i+1,strlen(src)*sizeof(int));
				i = 0;
			}
		}
		printf("-=Mini Shell %s =-*$",src);
		fgets(buf,sizeof(buf),stdin);
		*(buf+strlen(buf)-1) = '\0';
		argv[0] = buf;
		for (i = 0, j = 1; buf[i] != '\0'; i++)
		{
			if (buf[i] == ' ')
			{
				buf[i] = '\0';
				argv[j++] = buf+i+1;
			}
		}
		if (strcmp(buf,"quit") == 0)
		{
			break;
		}
		else if (strcmp(argv[0],"cd") == 0)
		{
			chdir(argv[1]);
		}
		else
		{
			fun(argv);
		}
	}
}
