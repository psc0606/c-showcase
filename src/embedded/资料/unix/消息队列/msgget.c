#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

int main()
{
	int msgid;
	key_t key;
	key = ftok(".",1);
	printf("%x\n",key);
	msgid = msgget(key,0);
	printf("%d\n",msgid);
}
