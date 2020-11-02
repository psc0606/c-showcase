#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

typedef struct
{
	long mtype;
	int len;
	char mtext[256];
}msg;

msg buf;
int msgid;

void sig_alarm(int signo)
{
	if (msgrcv(msgid,&buf,sizeof(buf),200,0) < 0)
	{
		perror("recv err");
	}
	printf("%s\n",buf.mtext);
}

int main()
{
	//int msgid;
	key_t key;
	key = ftok(".",1);
	printf("key = %x",key);
	
	//msg buf;
	buf.mtype = 100;
	msgid = msgget(key,IPC_CREAT | 0666);
	printf("msgid = %d\n",msgid);
	
	struct sigaction sig_act;
	sig_act.sa_handler = sig_alarm;
	if (sigaction(SIGUSR2, &sig_act,NULL) < 0)
	{
		perror("sigaction err");
		exit(0);
	}
	pid_t pid;
	printf("%d\n",getpid());
	scanf("%d",&pid);

	while (1)
	{	
		scanf("%s",buf.mtext);
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
		
		kill(pid,SIGUSR1);
	}
	msgctl(msgid,IPC_RMID,0);
}
