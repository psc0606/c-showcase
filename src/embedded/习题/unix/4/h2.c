/*----------------------------------------
**	2． 建立子进程，每秒钟向指定文件写入时间和序号信息例如
**	1 <time>
**	2 <time>
**	写入20次后退出。
**	要求：子进程打印序号，父进程打印时间。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd, i = 1;
	if (argc != 2)
	{
		perror("format err");
		exit(0);
	}
	if ((fd = open(argv[1],O_TRUNC | O_CREAT | O_RDWR, 0666)) < 0)
	{
		perror("open err");
		exit(0);
	}
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		perror("fork err");
		exit(0);
	}
	else if (pid == 0)
	{
		char buf[5];
		while (i <= 20)
		{
			snprintf(buf,sizeof(buf),"%2d. ",i++);
			write(fd,buf,strlen(buf));
			sleep(1);
		}
		exit(0);
	}
	else
	{
		time_t tm;
		char buf[128];
		while ((i++) <= 20)
		{
			sleep(1);
			time(&tm);
			snprintf(buf,sizeof(buf),"%s",ctime(&tm));
			write(fd,buf,strlen(buf));
		}
		exit(0);
	}
}
