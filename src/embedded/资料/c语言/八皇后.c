#include<stdio.h>
#include<stdlib.h>

int a[8][8];
int n = 0;

int judge(int i,int j)
{
	int k=1;
	for (k = 1; i >= k && j >= k; k++)
	{
		if (a[i-k][j-k])
		{
			return 0;
		}
	}
	for (k = 1; i >= k; k++)
	{
		if (a[i-k][j])
		{
			return 0;
		}
	}
	for (k = 1; i >= k && j+k < 8; k++)
	{
		if (a[i-k][j+k])
		{
			return 0;
		}
	}
	
	return 1;
}

void search(int i)
{
	int j;
	if(i==8)
	{
		printf("八皇后\n");
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				printf("%4d",a[i][j]);
			}
			printf("\n");
		}
		n++;
	}
	else
	{
		for(j = 0; j < 8; j++)
		{
			a[i][j] = 1;
			if(judge(i,j))
			{
				search(i+1);
			}
			a[i][j] = 0;
		}
	}
}

int main()
{
	search(0);
	printf("一共有%d种",n);
}
