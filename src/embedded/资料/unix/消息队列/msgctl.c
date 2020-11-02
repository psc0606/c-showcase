#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
	long mtype;
	int len;
	char mtext[32];
}msg;

int main()
{
	int msgid;
	key_t key;
	key = ftok(".",1);
	printf("%x\n",key);
	
	msgid = msgget(key,IPC_CREAT | 0666);
	printf("%d\n",msgid);
	
	struct msqid_ds buf;
	if (msgctl(msgid,IPC_STAT,&buf) < 0)
	{
		perror("ipc stat err");
		exit(0);
	}
	char *a = ctime(&buf.msg_ctime);
	printf("byte:%d, num:%d, time:%s",buf.msg_qbytes,buf.msg_qnum,a);
	
	msgctl(msgid,IPC_RMID,0);
}
