/*----------------------------------------
**	1. 实现“ls–l“的功能。
**	注意：需要判断是否为目录文件，若为目录，则列
**	出目录中所有文件的详细信息。
**----------------------------------------
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

void judge(struct stat buf)
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
	
	printf("\t%-5d",buf.st_nlink);
	printf("%-5d",buf.st_uid);
	printf("%-5d",buf.st_gid);
	printf("%-8d",buf.st_size);
	char *a = ctime(&buf.st_ctime);
	*(a+strlen(a)-1) = '\0';
	printf("%s",a);
}

int main(int argc,char **argv)
{
	struct stat buf;
	if (argc != 2)
	{
		perror("format err");
		exit(0);
	}
	if (stat(argv[1],&buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	if (S_ISREG(buf.st_mode))
	{
		judge(buf);
		printf("%10s\n",argv[1]);
	}
	if (S_ISDIR(buf.st_mode))
	{
		DIR *dir;
		struct dirent *dp;
		if ((dir = opendir(argv[1])) == NULL)
		{
			perror("open failed");
			exit(0);
		}
		chdir(argv[1]);
		while ((dp = readdir(dir)) != NULL)
		{
			if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
			{
				continue;
			}
			if (stat(dp->d_name,&buf) < 0)
			{
				perror("err");
				exit(0);
			}
			judge(buf);
			printf("%10s\n",dp->d_name);
		}
	}
}
