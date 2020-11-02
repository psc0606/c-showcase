#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
	pid_t pid;
	int fd[2];
	char r_buf[128] = {0};
	char w_buf[128] = {0};
	if (pipe(fd) < 0)
	{
		printf("pipe create error\n");
		return -1;
	}
	if ((pid = fork()) == 0)
	{
		close(fd[1]);
		sleep(3);
		int r_num = 0;
		r_num = read(fd[0],r_buf,sizeof(r_buf));
		printf("read num is %d\n",r_num);
		close(fd[0]);
		exit(0);
	}
	else if (pid > 0)
	{
		//close(fd[0]);
		/*strcpy(w_buf,"111");
		if (write(fd[1],w_buf,4) == -1)
		{
			perror("write err");
		}*/
		/*close(fd[1]);
		printf("parent close fd[1] over\n");
		wait(NULL);
		*/
		int n, i = 0;
		
		while (1)
		{
			if ((n = write(fd[1],w_buf,1024)) == -1)
			{
				perror("write err");
				break;
			}
			i++;
			printf("i = %d, n = %d\n",i,n);
		}
		close(fd[1]);
		close(fd[0]);//阻塞
		printf("parent close fd[1] over\n");
		wait(NULL);
	}
}
