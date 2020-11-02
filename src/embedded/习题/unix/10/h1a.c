/*1. 利用消息队列和线程，实现QQ的基本功能，能够互通消息。要求：使用两个终端进行消息的传输，在发送和接收消息时，互不干扰。提示：一个线程负责读，一个线程负责写*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <string.h>

typedef struct
{
	long mtype;
	int len;
	char mtext[256];
}msg;

void *fun1(void *arg)
{
	int msgid = *(int *)arg;
	printf("%d\n",msgid);
	msg buf;
//	buf.mtype = 100;
	while (1)
	{
		if (msgrcv(msgid,&buf,sizeof(buf),100,0) < 0)
		{
			perror("msgrcv err");
		}
		printf("%s\n",buf.mtext);
	}
}

void *fun2(void *arg)
{
	int msgid = *(int *)arg;
	printf("%d\n",msgid);

	msg buf;
	buf.mtype = 200;
	while (1)
	{
		scanf("%s",buf.mtext);
		buf.len = strlen(buf.mtext);
		if (strcmp("quit",buf.mtext) == 0)
		{
			break;
		}
		if (msgsnd(msgid,&buf,sizeof(buf),0) < 0)
		{
			perror("msgrcv err");
		}
	}
}

int main()
{
	pthread_t tid1,tid2;
	key_t key;
	if ((key = ftok(".",1)) < 0)
	{
		perror("ftok err");
		exit(0);
	}
	int msgid;
	if ((msgid = msgget(key,IPC_CREAT | 0666)) < 0)
	{
		perror("msgget err");
		exit(0);
	}

	if (pthread_create(&tid1,NULL,fun1,&msgid) != 0)
	{
		perror("create pthread 1 err");
		exit(0);
	}
	if (pthread_create(&tid2,NULL,fun2,&msgid) != 0)
	{
		perror("create pthread 2 err");
		exit(0);
	}

	pthread_join(tid2,NULL);
	pthread_cancel(tid1);
	if (msgctl(msgid,IPC_RMID,NULL) < 0)
	{
		perror("msgctl err");
		exit(0);
	}	
}
