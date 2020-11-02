#include<stdio.h>
int main()
{
	int a[10],i,j,t;
	int n;
	scanf("%d\n",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	for(j=0;j<n-1;j++)
	{
		for(i=0;i<n-1-j;i++)
		{
			if(a[i]<a[i+1])
			{
				t=a[i];
				a[i]=a[i+1];
				a[i+1]=t;
			}
		}
	}
	for(i=0;i<n;i++)
	{
		if(a[i]==a[i+1])
		{
			continue;
		}
		printf("%d\t",a[i]);
	}
}
