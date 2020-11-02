#include<stdio.h>
int main()
{
	int i,j;
	for(i=1;i<=7;i++)
	{
		for(j=0;j<4-((i<=4)?i:8-i);j++)
			printf(" ");
		for(j=0;j<2*((i<=4)?i:8-i)-1;j++)
			printf("*");
		printf("\n");
	}
}
