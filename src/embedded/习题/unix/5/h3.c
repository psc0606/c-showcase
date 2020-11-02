/*----------------------------------------
**	3.编写代码，生成可执行程序f，然后如下调用f：
**	./f   filename
**	filename 如果是文件名，则显示出文件信息。类似“ls –l ”
**	filename 如果是目录名，则显示出该路径下所有文件信息。
**	不递归进入其子目录。
**	如若有多个文件或目录，则显示多个文件或目录对应的信息
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void display(char *filename, struct stat buf)
{
	if (S_ISREG(buf.st_mode))
	{
		printf("-");
	}
	else if (S_ISDIR(buf.st_mode))
	{
		printf("d");
	}
	
	if ((S_IRUSR & buf.st_mode) == S_IRUSR)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if ((S_IWUSR & buf.st_mode) == S_IWUSR)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if ((S_IXUSR & buf.st_mode) == S_IXUSR)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
//************************
	if ((S_IRGRP & buf.st_mode) == S_IRGRP)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if ((S_IWGRP & buf.st_mode) == S_IWGRP)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if ((S_IXGRP & buf.st_mode) == S_IXGRP)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
//**************************
	if ((S_IROTH & buf.st_mode) == S_IROTH)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if ((S_IWOTH & buf.st_mode) == S_IWOTH)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if ((S_IXOTH & buf.st_mode) == S_IXOTH)
	{
		printf("x.");
	}
	else
	{
		printf("-.");
	}
	
	printf(" %d ",buf.st_nlink);
	printf(" %d",buf.st_uid);
	printf(" %d",buf.st_gid);
	printf(" %6d",buf.st_size);
	char *a = ctime(&buf.st_ctime);
	*(a+strlen(a)-1) = '\0';
	printf("  %s",a);
}

void ls(char *argv)
{
	DIR *dir;
	struct stat buf;
	struct dirent *dp;
	char data[256];
	getcwd(data,sizeof(data));
	if (stat(argv,&buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	if (S_ISREG(buf.st_mode))
	{
		display(argv,buf);
		printf("  %s\n",argv);
	}
	else if (S_ISDIR(buf.st_mode))
	{
		if ((dir = opendir(argv)) == NULL)
		{
			perror("opendir err");
		}
		chdir(argv);
		while ((dp = readdir(dir)) != NULL)
		{
			if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
			{
				continue;
			}
			if (stat(dp->d_name,&buf) < 0)
			{
				perror("stat err");
				exit(0);
			}
			display(dp->d_name,buf);
			printf("  %s\n",dp->d_name);
		}
		printf("\n");
		chdir(data);
	}
}

int main(int argc, char **argv)
{
	int i = 1;
	while (i < argc)
	{
		ls(argv[i++]);
	}
}
