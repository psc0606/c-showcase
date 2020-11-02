/*----------------------------------------
**	1． 一个父进程创建两个子进程，两个子进程同时
**	处理一个文件，子进程A负责把文本中的小写字母
**	换成大写字母，子进程B负责吧文本中的非字母换
**	成空格，然后子进程A、B分别把对大小写字母和
**	数字的统计结果写入一个特定文件。子进程退出
**	后由父进程读出特定文件中的统计数据打印到终端。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void change(int fd,int *num_w,int *num_W)
{
	int i, n;
	char buf[1024] = {0};
	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		for (i = 0; buf[i] != '\0'; i++)
		{
			if (buf[i] >= 'A' && buf[i] <= 'Z')
			{
				(*num_W)++;
			}
			else if (buf[i] >= 'a' && buf[i] <= 'z')
			{
				buf[i] -= 32;
				(*num_w)++;
			}
		}
		lseek(fd, -n, SEEK_CUR);
		write(fd, buf, n);
	}
}

void space(int fd, int *num_n)
{
	int i, n;
	char buf[1024] = {0};
	while ((n = read(fd, buf, sizeof(buf))) > 0)
	{
		for (i = 0; buf[i] != '\0'; i++)
		{
			if (!((buf[i] >= 'a' && buf[i] <= 'z') || (buf[i] >= 'A' && buf[i] <= 'Z')
					|| buf[i] == '\n'))
			{
				if (buf[i] >= '0' && buf[i] <= '9')
				{
					(*num_n)++;
				}
				buf[i] = ' ';
			}
		}
		lseek(fd, -n, SEEK_CUR);
		write(fd, buf, n);
	}
}


int main(int argc, char **argv)
{
	int fd1, fd2, status, n;
	char data[256] = {0};
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if ((fd1 = open(argv[1],O_RDWR)) < 0)
	{
		perror("open err");
		exit(0);
	}
	if ((fd2 = open(argv[2],O_CREAT | O_TRUNC | O_RDWR, 0666)) < 0)
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
		int num_w = 0, num_W = 0;
		change(fd1,&num_w,&num_W);
		snprintf(data,sizeof(data),"小写字母有%d个\n",num_w);
		write(fd2,data,strlen(data));
		snprintf(data,sizeof(data),"大写字母有%d个\n",num_W);
		write(fd2,data,strlen(data));	
	}
	else
	{
		pid_t pid1;
		if ((pid1 = fork()) < 0)
		{
			perror("fork 1 err");
			exit(0);
		}
		else if (pid1 == 0)
		{
			sleep(1);
			int num_n = 0;
			lseek(fd1,0,SEEK_SET);
			space(fd1, &num_n);
			snprintf(data,sizeof(data),"数字有%d个\n",num_n);
			write(fd2,data,strlen(data));
		}
		else
		{
			waitpid(pid1,&status,0);
			lseek(fd2, 0, SEEK_SET);
			n = read(fd2,data,sizeof(data));
			write(1,data,n);
		}
	}
}
