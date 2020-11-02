#include<stdio.h>
int main()
{
	int i,j;
	int n;
	printf("请输入行数：");
	scanf("%d",&n);
	for(i=1;i<=n;i++)
	{
		for(j=0;j<(n+1)/2-((i<=(n+1)/2)?i:n+1-i);j++)
			printf(" ");
		for(j=0;j<2*((i<=(n+1)/2)?i:n+1-i)-1;j++)
			printf("*");
		printf("\n");
	}
}




/*int main()
{
	int a[7][7];
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<7;j++)
		{
			if(j>=3-i && j<=3+i)
			{
				a[i][j] = '*';
			}
			else
			{
				a[i][j] = ' ';
			}
		}
	}
	for(i=4;i<7;i++)
	{
		for(j=0;j<7;j++)
		{
			if(j>=i-3 && j<=9-i)
			{
				a[i][j] = '*';
			}
			else
			{
				a[i][j] = ' ';
			}		
		}
	}
	for(i=0;i<7;i++)
	{
		for(j=0;j<7;j++)
		{	
			printf("%c",a[i][j]);
		}
		printf("\n");
	}	
}*/
