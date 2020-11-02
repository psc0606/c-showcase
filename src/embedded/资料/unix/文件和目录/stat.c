#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct stat buf;
	if (argc != 2)
	{
		perror("format err!\n");
		return -1;
	}
	if (stat(argv[1], &buf) < 0)
	{
		perror("stat failed!\n");
		return -1;
	}
	
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
//*************************************************	
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
//******************************************
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
		printf("x");
	}
	else
	{
		printf("-.");
	}	
	
	printf("%5ld",buf.st_size);
	char *a = ctime(&buf.st_mtime);
	*(a+strlen(a)-1) = '\0';
	printf("\t%s",a);
	printf("%5ld",buf.st_uid);
	printf("%5ld",buf.st_gid);
	printf("%10s\n",argv[1]);	
}
