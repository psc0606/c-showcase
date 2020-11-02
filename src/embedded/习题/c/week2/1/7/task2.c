#include<stdio.h>
int main()
{
	int a[100][100];
	int n,i,j,x=0,y,m=1;
	printf("please input n:");
	scanf("%d",&n);
	y=n/2;
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			a[x][y]=m;
			x--;
			y++;
			m++;
			if(x == -1)
				x=n-1;
			if(y == n)
				y= 0;
		}
		x++;
		if(x>= n)
			x=0;
		x++;
		y--;
		if(x>= n)
			x=0;
		if(y== -1)
			y=n-1;
	}
	printf("魔方阵:\n");
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			printf("%4d",a[i][j]);
		}
		printf("\n");
	}
}
