#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{
	pid_t pid ;
	pid = fork();
	if(pid < 0)
	{
		perror("fork err.");
		return -1;
	}
	else if(pid == 0)
	{
		pid_t pid2;
		pid2 = fork();
		if(pid2 < 0 )
		{
			perror("fork 2 err.");
			exit(1);
		}
		else if(pid2 == 0)
		{
			sleep(1);
			printf("pid 2 :%d\n",getpid());	
		}
		else if(pid2 > 0)
		{
			sleep(3);
			printf("pid  %d exit.\n",getpid());
			exit(1);
		}
	}
	else if( pid > 0)
	{
		pid_t pid1;
		pid1 = fork();
		if(pid1 < 0)
		{
			perror("fork1 err.");
			exit(1);
		}
		else if(pid1 == 0)
		{
			sleep(2);
			printf("pid 1 %d exit .\n",getpid());
			exit(2);
		}
		else
		{
			int status;
			pid_t p ;
			while((p = waitpid(pid1,&status,WNOHANG)) != pid1 )
			{
				printf("waiting...\n");
				sleep(1);
			}
			if(WIFEXITED(status))
				printf("wait pid %d, status: %d\n",p,WEXITSTATUS(status));
			printf("father exit.\n");
			exit(0);
		}
	}
}
