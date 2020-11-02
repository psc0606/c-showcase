#include<stdio.h>

int gongyue(int m,int n)
{
	int t,r;
	if(m<n)
	{
		t=m;
		m=n;
		n=t;
	}
	while(m%n!=0)
	{
		r=m%n;
		m=n;
		n=r;
	}
	return n;
}

int main()
{
	int a[10];
	int i,j;
	long long int k=1;
	for(i=0;i<10;i++)
	{
		scanf("%d",&a[i]);
		k*=a[i];
	}
	for(i=0;i<9;i++)
	{
		for(j=0;j<9-i;j++)
		{
			a[j]=gongyue(a[j],a[j+1]);
			//printf("%-2d",a[j]);
		}
		//printf("\n");
	}
	printf("最大公约数:%d,最小公倍数:%lld",a[j],k/a[j]);
}
