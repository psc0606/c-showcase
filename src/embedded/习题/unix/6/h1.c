/*----------------------------------------
**	1. 实现ls –l | grep ‘if’ ,该shell命令利
**	用管道实现的例子，它的含义为：在ls –l列表中
**	查找if关键字。
**----------------------------------------
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "pipe.c"

void judge(struct stat buf,char w_buf[1024])
{
	if (S_ISREG(buf.st_mode))
	{
		printf("-");
	}
	if (S_ISDIR(buf.st_mode))
	{
		printf("d");
	}
	if ((buf.st_mode & S_IRUSR) == S_IRUSR)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if ((buf.st_mode & S_IWUSR) == S_IWUSR)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if ((buf.st_mode & S_IXUSR) == S_IXUSR)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
//****************************************
	if ((buf.st_mode & S_IRGRP) == S_IRGRP)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if ((buf.st_mode & S_IWGRP) == S_IWGRP)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if ((buf.st_mode & S_IXGRP) == S_IXGRP)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
//*******************************************
	if ((buf.st_mode & S_IROTH) == S_IROTH)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if ((buf.st_mode & S_IWOTH) == S_IWOTH)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if ((buf.st_mode & S_IXOTH) == S_IXOTH)
	{
		printf("x.");
	}
	else
	{
		printf("-.");
	}
	
	strcat(w_buf,buf.st_nlink);
	strcat(w_buf,buf.st_uid);
	strcat(w_buf,buf.st_gid);
	strcat(w_buf,buf.st_size);
	char *a = ctime(&buf.st_ctime);
	*(a+strlen(a)-1) = '\0';
	strcat(w_buf,a);
}

int main(int argc,char **argv)
{
	if (argc != 2)
	{
		perror("format err");
		exit(0);
	}
	int fd[2], pid;
	if (pipe(fd) < 0)
	{
		perror("pipe err");
		exit(0);
	}
	if ((pid = fork()) < 0)
	{
		perror("fork err");
		exit(0);
	}
	else if (pid == 0)
	{
		DIR *dir;
		struct dirent *dp;
		struct stat buf;
		char w_buf[1024];
		close(fd[0]);
		if ((dir = opendir(".")) == NULL)
		{
			perror("opendir err");
			exit(0);
		}
		while ((dp = readdir(dir) != NULL)
		{
			if (((strcmp(dp->d_name,".") == 0)) || (strcmp(dp->d_name,"..") == 0))
			{
				continue;
			}
			if (stat(dp->d_name,&buf) < 0)
			{
				perror("stat err");
				exit(0);
			}
			memset(buf,0,sizeof(buf));
			tell_parent();
			judge(buf,w_buf);
			strcat(w_buf,dp->d_name);
		}
	}
	else
	{
		close(fd[1]);
		tell_
	}
}
