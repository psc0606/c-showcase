#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<time.h>
#include"pipe.c"

char* itoc(int n)
{
	char* ptr =malloc(sizeof(char)*5);
	char ch;
	int len,i=0;
	memset(ptr,0,sizeof(char)*5);
	while(n)
	{
		*(ptr+i) = '0'+n%10;
		n = n/10;
		i++;
	}

	len = strlen(ptr);
	for(i =0;i<len/2;i++)
	{
		ch = ptr[i];
		ptr[i] = ptr[len-i-1];
		ptr[len-i-1] = ch;
	}
	return ptr;
}

int main()
{
	pid_t pid;
	int fd;
	int i = 1;
	fd = open("2.txt",O_CREAT|O_RDWR,0666);
	if(fd < 0)
	{
		perror("fd open failed");
		exit(1);
	}
	init_pipe();
	pid = fork();
	if(pid < 0)
	{
		perror("fork err.");
		exit(1);
	}
	else if(pid == 0)
	{
		char* ptr;
		printf("child %d\n",i);
		while(i <= 20)
		{
			ptr = itoc(i);
			if(write(fd,ptr,strlen(ptr)+1) != strlen(ptr)+1)
			{
				perror("write err:");
				exit(1);
			}
			
			tell_father();
			wait_father();
			sleep(1);
			free(ptr);
			i++;
		}
		tell_father();
	}
	else if(pid > 0)
	{
		time_t tm;
		char* ptr;
		
		while(i <= 20)
		{
			wait_child();
			time(&tm);
			ptr = ctime(&tm);
			if(write(fd,ptr,strlen(ptr))!=strlen(ptr))
			{
				perror("father write err.");
				exit(1);
			}
			printf("%s\n",ptr);
			tell_child();
			i++;
		}
		wait(NULL);
	}
	
}
