#include<stdio.h>
void fun(int *p,int n)
{
	int i,t;
	t=p[0];
	for(i=0;i<n-1;i++)
	{
		p[i]=p[i+1];
	}
	p[i]=t;
}

int main()
{
	int a[100],m=1,i,j,n;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		a[i]=m++;
	}
	/*fun(a,n);
	for(i=0;i<n;i++)
	{
		printf("%d",a[i]);
	}*/
	for(j=0;j<n;j++)
	{
		for(i=0;i<n;i++)
		{
			printf("%d ",a[i]);
		}
		printf("\n");
		fun(a,n);
	}
}
