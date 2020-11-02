/*----------------------------------------
**	 4.编写代码，生成可执行程序f，然后如下调用f：
**	./f   exps  filename
**	exps 和filename是一个关键字和一个目录名或
**	文件名。如果filename是目录，该程序实现在
**	filename中查找exps关键字，而filename中可
**	能有文件有子目录，其子目录中可能又有目录，目
**	录中又有文件和目录，要求打印出所有包含exps
**	关键字文件的绝对路径，和关键字所在行数。如果
**	filename是文件，则查找关键字在文件中的行数。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

int search_file(char *exps,char *argv)
{
	FILE *fp;
	int i, n = strlen(exps), num = 0, flag = 0;
	char buf[1024], a[256], b[10] = {0};
	if ((fp = fopen(argv,"r")) ==NULL)
	{
		perror("open err");
		exit(0);
	}
	printf("%s/%s\n",getcwd(a,sizeof(a)),argv);
	while (1)
	{
		fgets(buf,sizeof(buf),fp);
		if (feof(fp))
		{
			break;
		}
		num++;
		for (i = 0; buf[i] != '\n'; i++)
		{
			if (*exps == buf[i])
			{
				memcpy(b,(buf+i),n*sizeof(char));
				if (strcmp(exps,b) == 0)
				{
					flag = 1;
					printf("the word is %d line\n",num);
				}
			}
		}
	}
	if(flag == 0)
	{
		printf("no find!\n");
	}
}

void search_dir(char *exps,char *argv)
{
	DIR *dir;
	struct dirent *dp;
	if ((dir = opendir(argv)) == NULL)
	{
		perror("open err");
		exit(0);
	}
	chdir(argv);
	while ((dp = readdir(dir)) != NULL)
	{
		if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
		{
			continue;
		}
		if (DT_REG == dp->d_type)
		{
			search_file(exps,dp->d_name);
		}
		else if (DT_DIR == dp->d_type)
		{
			search_dir(exps,dp->d_name);
			chdir("..");
		}
	}
}

int main(int argc, char **argv)
{
	struct stat buf;
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if (stat(argv[2],&buf) < 0)
	{
		perror("stat err");
		exit(0);
	}
	if (S_ISREG(buf.st_mode))
	{
		search_file(argv[1],argv[2]);
	}
	else if (S_ISDIR(buf.st_mode))
	{
		search_dir(argv[1],argv[2]);
	}
}
