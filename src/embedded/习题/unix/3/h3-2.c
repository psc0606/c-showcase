#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int cp_file(char *src, char *dst)
{
	int fd1, fd2, n;
	char data[8192];
	struct stat buf;
	if (stat(src, &buf) < 0)
	{
		perror("stat failed");
		return -1;
	}
	if ((fd1 = open(src,O_RDWR)) < 0)
	{
		perror("open failed");
		return -1;
	}
	umask(0000);
	if ((fd2 = open(dst,O_CREAT | O_RDWR,buf.st_mode & 0777)) < 0)
	{
		perror("open failed");
		return -1;
	}
	while ((n = read(fd1, data, sizeof(data))) > 0)
	{
		write(fd2, data, n);
	}
}

int cp_dir(char *src, char *dst)
{
	DIR *dir;
	char path_src[256] = {0}, path_dst[256] = {0};
	struct stat buf;
	struct dirent *dp;
	strcpy(path_src,src);
	strcpy(path_dst,dst);
	if (stat(src, &buf) < 0)
	{
		perror("stat failed");
		return -1;
	}
	if ((dir = opendir(path_src)) == NULL)
	{
		perror("open err");
		return -1;
	}
	umask(0000);
	if (mkdir(path_dst,buf.st_mode & 0777) < 0)
	{
		perror("mkdir err");
		return -1;
	}
	while ((dp = readdir(dir)) != NULL)
	{
		strcpy(path_src,src);
		strcpy(path_dst,dst);
		if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
		{
			continue;
		}
		strcat(path_src,"/");
		strcat(path_dst,"/");
		strcat(path_src,dp->d_name);
		strcat(path_dst,dp->d_name);
		
		if (DT_REG == dp->d_type)
		{
			cp_file(path_src, path_dst);
		}
		else if (DT_DIR == dp->d_type)
		{
			cp_dir(path_src, path_dst);
		}
	}
}

int main(int argc, char **argv)
{
	struct stat buf;
	if (argc != 3)
	{
		perror("format err");
		return -1;
	}
	if (stat(argv[1],&buf) < 0)
	{
		perror("input err");
		return -1;
	}
	if (S_ISREG(buf.st_mode))
	{
		cp_file(argv[1],argv[2]);
	}
	else if (S_ISDIR(buf.st_mode))
	{
		cp_dir(argv[1],argv[2]);
	}
}
