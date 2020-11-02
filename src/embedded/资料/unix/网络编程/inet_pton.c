#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	struct in_addr addr;
	
	inet_pton(AF_INET,argv[1],&addr);
	
	printf("ptr:%s\naddr:%x\n",argv[1],addr);
}
