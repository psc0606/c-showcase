#include<stdio.h>

long int fun(int x)
{
	long int m=1;
	while(x!=0)
	{
		m=m*10;
		x--;
	}
	return m;
}

int main()
{
	int a,n,i,j;
	long int k=0,sum=0;
	scanf("%d,%d",&a,&n);
	for(i=1;i<=n;i++)
	{
		j=i;
		k=0;
		while(j-1>=0)
		{
			k+=a*fun(j-1);
			j--;
		}
		//printf("%ld\n",k);
		sum+=k;
	}
	printf("%ld",sum);
}
