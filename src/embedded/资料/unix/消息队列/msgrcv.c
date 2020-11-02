#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

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
	
	while (1)
	{
		scanf("%d %s",&buf.mtype,buf.mtext);
		buf.len = strlen(buf.mtext);
		if (strcmp(buf.mtext,"quit") == 0)
		{
			break;
		}
		if (msgsnd(msgid,&buf,sizeof(buf),0) < 0)
		{
			perror("msg snd failed.");
			exit(0);
		}
	}
	
	long type;
	printf("recv type:");
	scanf("%d",&type);
	if (msgrcv(msgid,&buf,sizeof(buf),type,0) < 0)
	{
		perror("recv err");
	}
	printf("recv:%s\n",buf.mtext);
}
