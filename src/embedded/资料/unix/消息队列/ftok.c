#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
	key_t key;
	int i;
	key = ftok(".",2);
	printf("key is %x\n",key);
	struct stat buf;
	stat(".",&buf);
	printf("%x,%x\n",buf.st_ino,buf.st_dev);
}
/*
int main()
{
	int msgid;
	key_t key;
	msgid = msgget(IPC_PRIVATE,IPC_CREAT | 0666);
	printf("%d\n",msgid);
}
*/

/*
int main()
{
	int msgid;
	key_t key;
	key = ftok(".",1);
	printf("%x\n",key);
	msgid = msgget(key,IPC_CREAT | 0666);
	printf("%d\n",msgid);
}
*/

/*
int main()
{
	int msgid;
	key_t key;
	key = ftok(".",1);
	printf("%x\n",key);
	//msgid = msgget(key,0);
	msgid = msgget(IPC_PRIVATE,0);
	printf("%d\n",msgid);
}

*/
