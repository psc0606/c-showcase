#include<stdio.h>
int main()
{
	int n;
	int a[100][100];
	int i,j,k;
	printf("please input a number:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		a[i][0]=1;
		a[i][i]=1;
	}
	for(i=0;i<n;i++)
	{
		for(j=1;j<i;j++)
		{
			a[i][j]=a[i-1][j-1]+a[i-1][j];
		}
	}
	for(i=0;i<n;i++)
	{
		for(k=0;k<n-1-i;k++)
			printf("  ");
		for(j=0;j<=i;j++)
		{
			printf("%-4d",a[i][j]);
		}
		printf("\n");
	}
}


/*int fun(int x,int y)
{
	int z;
	if(y==1 || y==x+1)
		return 1;
	z=fun(x-1,y-1)+fun(x-1,y);
	return z;
}

int main()
{
	int i,j,n=13;
	printf("N=");
	while(n>12)
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		for(j=0;j<12-i;j++)
		{
			printf(" ");
		}
		for(j=1;j<=i+1;j++)
		{
			printf("%6d",fun(i,j));
		}
		printf("\n");
	}
}*/
