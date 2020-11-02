#include<stdio.h>
int main()
{
	int n,i,j,k;
	int a[100][100];
	scanf("%d",&n);
	k=n;
	while(n>=1)
	{
		i=n-1;
		for(j=n-1;j<n-1+2*(k-n)+1;j++)
		{
			a[i][j] = n;
		}
		j--;
		for(i=n-1;i<n-1+2*(k-n)+1;i++)
		{
			a[i][j] = n;
		}
		i--;
		for(;j>=n-1;j--)
		{
			a[i][j] = n;
		}
		j++;
		for(;i>n-1;i--)
		{
			a[i][j] = n;
		}
		n--;
	}
	for(i=0;i<2*k-1;i++)
	{
		for(j=0;j<2*k-1;j++)
		{
			printf("%-3d",a[i][j]);
		}
		printf("\n");
	}
}
