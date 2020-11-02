#include<stdio.h>
#include<string.h>
int main()
{
	int n,a[100],m,i,j,temp;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	printf("please insert m:");
	scanf("%d",&m);
	a[n]=m;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n-i;j++)
		{
			if(a[j]>a[j+1])
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	for(i=0;i<=n;i++)
	{
		printf("%d",a[i]);
	}
}
