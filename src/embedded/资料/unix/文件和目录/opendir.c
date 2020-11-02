#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc,char **argv)
{
	DIR *dir;
	struct dirent *dp;
	if ((dir = opendir(argv[1])) == NULL)
	{
		perror("open dir err.");
		return -1;
	}
	while ((dp = readdir(dir)) != NULL)
	{
		if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
		{
			continue;
		}
		printf("%s\t",dp->d_name);
	}
}
