#include<stdio.h>
int fun(int x)
{
	int i,k;
	for(i=2;i<x;i++)
	{
		if((k=x%i)==0)
			break;
	}
	if(i==x)
	{
		printf("%d",x);
	}
	else
	{
		printf("%d*",i);
		fun(x/i);
	}
}

int main()
{
	int n;
	printf("please input a number:");
	scanf("%d",&n);
	fun(n);
}
