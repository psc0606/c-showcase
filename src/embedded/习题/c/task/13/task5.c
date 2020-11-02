#include<stdio.h>
int main()
{
	int a[100][100];
	int n;
	printf("please input n:");
	scanf("%d",&n);
	int len=n*n,row=0,col=0,i=0,k,j;
	int circle=0;
	while(i<len)
	{
		for(;col<n-circle;col++)
			a[row][col]=++i;
		col--;
		row++;
		for(;row<n-circle;row++)
			a[row][col]=++i;
		row--;
		col--;
		for(;col>=circle;col--)
			a[row][col]=++i;
		col++;
		row--;
		for(;row>circle;row--)
			a[row][col]=++i;
		row++;
		col++;
		circle++;		
	}
	
	for(k=0;k<n;k++)
	{
		for(j=0;j<n;j++)
			printf("%-5d",a[k][j]);
		printf("\n");
	}
}
