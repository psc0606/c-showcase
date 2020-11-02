/*----------------------------------------
**	3. 编写程序实现目录的拷贝，例如运行 
**	 ./f  A   B   则实现A目录到B 目录的完全
**	拷贝。需要注意的是，A目录可能不是一个简单的
**	目录，它可能包含有其他文件或者目录。你可能会
**	用到mkdir（）函数。
**----------------------------------------
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void copydir(char *fname1,char *fname2, char *b1, char *b2)
{
	int fd1, fd2, n;
	DIR *dir;
	struct dirent *dp;
	struct stat buf;
	char buf1[30], buf2[30], buf3[1024];
	strcpy(buf1,b1);
	strcpy(buf2,b2);
	strcat(buf1,"/");
	strcat(buf2,"/");
	strcat(buf1,fname1);
	strcat(buf2,fname2);
	
	if ((dir = opendir(fname1)) == NULL)
	{
		perror("open err");
		return;
	}
	
	chdir(buf1);
	while ((dp = readdir(dir)) != NULL)
	{
		
		if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0))
		{
			continue;
		}
		
		if (stat(dp->d_name, &buf) < 0)
		{
			perror("stat err");
			return;
		}
		
		if (DT_REG == dp->d_type)
		{
			fd1 = open(dp->d_name, O_RDONLY);
			chdir(buf2);
			umask(0000);
			fd2 = open(dp->d_name,O_RDWR | O_CREAT, buf.st_mode & 0777);
			chdir(buf1);
			while (1)
			{
				if ((n = read(fd1, buf3, sizeof(buf3))) == 0)
				{
					break;
				}
				chdir(buf2);
				write(fd2, buf3, n);
				chdir(buf1);
			}
		}
		else
		{
			chdir(buf2);
			umask(0000);
			mkdir(dp->d_name, buf.st_mode & 0777);
			chdir(buf1);
			copydir(dp->d_name, dp->d_name, buf1, buf2);
			chdir("..");
		}
	}
}

int main(int argc, char **argv)
{
	char b[30];
	struct stat buf;	
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if (stat(argv[1], &buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	if (S_ISDIR(buf.st_mode))
	{
		mkdir(argv[2], buf.st_mode & 0777);
		getcwd(b,sizeof(b));
		copydir(argv[1], argv[2], b, b);
	}
	else
	{
		perror("input format err");
		exit(0);
	}
}
