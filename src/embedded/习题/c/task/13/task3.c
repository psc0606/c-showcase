#include<stdio.h>
int main(int argc,char **argv)
{
	if(argc==3)
		printf("Do you %s %s\n",argv[1],argv[2]);
	if(argc==4)
		printf("Do you %s %s %s\n",argv[1],argv[2],argv[3]);
	if(argc>5)
		printf("Do you %s %s ,%s,%s or %s\n",argv[1],argv[2],argv[3],argv[4],argv[5]);
}
