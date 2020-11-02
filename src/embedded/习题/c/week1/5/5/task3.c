#include<stdio.h>
int main()
{
	int i,j,sum=0,n=0;
	for(i=0;i<=4;i++)
	{
		for(j=0;j<=3;j++)
		{
			if(!(i==0 && j==0))
			{
				sum=3*i+5*j;
				n++;
				printf("%d\n",sum);
			}
		}
	}
	printf("一共有%d种",n);
}
