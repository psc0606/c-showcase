#include<stdio.h>
int main()
{
	int i,j;
	int n;
	printf("请输入行数：");
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		for(j=0;j<(n+1)/2-((i<=(n+1)/2)?i:n+1-i);j++)
			printf(" ");
		for(j=0;j<2*((i<=(n+1)/2)?i:n+1-i)-1;j++)
			printf("*");
		printf("\n");
	}
}
