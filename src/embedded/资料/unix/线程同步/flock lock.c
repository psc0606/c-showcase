#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void my_lock(int fd)
{
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	fcntl(fd,F_SETLKW,&lock);
}

void my_unlock(int fd)
{
	struct flock lock;
	lock.l_type = F_UNLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	fcntl(fd,F_SETLK,&lock);
}

#define MAXLINE 1023

void *fun()
{
	int fd;
	long i, seqno;
	pid_t pid;
	ssize_t n;
	char line[MAXLINE-1];
	pthread_t tid = pthread_self();
	pid = getpid();
	fd = open("111",O_RDWR,0);
	//my_lock(fd);
	for (i = 0; i < 20; i++)
	{
		lseek(fd,0l,SEEK_SET);
		n = read(fd,line,MAXLINE);
		line[n] = 0;
		n = sscanf(line,"%ld\n",&seqno);
		printf("pid = %ld,tid = Ox(%x),seq# = %ld\n",pid,tid,seqno);
		seqno++;
		snprintf(line,sizeof(line),"%ld\n",seqno);
		lseek(fd,0l,SEEK_SET);
		write(fd,line,strlen(line));
		sleep(1);
	}
	//my_unlock(fd);
}

int main(int argc,char **argv)
{
	pthread_t tid1,tid2;
	if (pthread_create(&tid1,NULL,fun,NULL) < 0)
	{
		perror("create err");
		exit(0);
	}
	if (pthread_create(&tid2,NULL,fun,NULL) < 0)
	{
		perror("create err");
		exit(0);
	}
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	/*pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		fun();
	}
	else if (pid > 0)
	{
		fun();
		wait(NULL);
	}*/
}
