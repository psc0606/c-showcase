#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	int a = 0x01234567;
	char *p = (char *)&a;
	int i;
	for (i = 0; i < 4; i++)
	{
		printf("%x",p[i]);
	}
}
