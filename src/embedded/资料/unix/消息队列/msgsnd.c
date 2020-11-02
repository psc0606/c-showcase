#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/msg.h>

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
	
	msgid = msgget(key,0);
	printf("%d\n",msgid);
	msg buf;
	buf.mtype = 100;
	strcpy(buf.mtext,"123456789");
	buf.len = strlen(buf.mtext);
	
	if (msgsnd(msgid,&buf,sizeof(buf),0) < 0)
	{
		perror("msg snd failed.");
		exit(0);
	}
}
