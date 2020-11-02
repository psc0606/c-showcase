/*----------------------------------------
**	2. 编写代码，生成可执行程序f，然后如下调用f：
**	./f   filename  directory
**	filename 和directory分别是一个文件名路径名
**	和一个目录路径名。该程序实现在directory中查找
**	filename，而directory中可能有文件有子目录，
**	其子目录中可能又有目录，目录中又有文件和目录…，
**	若存在，打印出相对路径。
**----------------------------------------
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void judge(char *a,char *b)
{
	char buf[30];
	DIR *dir;
	struct dirent *dp;
	if ((dir = opendir(b)) == NULL)
	{
		perror("open dir err");
		return;
	}
	chdir(b);
	while ((dp = readdir(dir)) != NULL)
	{
		if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
		{
			continue;
		}
		
		if (DT_REG == dp->d_type)
		{
			if (strcmp(a,dp->d_name) == 0)
			{
				printf("%s\n",getcwd(buf,sizeof(buf)));
			}
		}
		else if (DT_DIR == dp->d_type)
		{
			judge(a,dp->d_name);
			chdir("..");
		}
	}
}

int main(int argc,char **argv)
{
	struct stat buf;
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if (stat(argv[2],&buf) < 0)
	{
		perror("1 err");
		exit(0);
	}
	if (S_ISDIR(buf.st_mode))
	{
		judge(argv[1],argv[2]);
	}
}
