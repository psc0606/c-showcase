#include<stdio.h>
#include<stdlib.h>

#define N 8
int a[N][N];

int Valid(int i,int j)
{
	int k=1;
	for(k=1;i>=k&&j>=k;k++)
		if(a[i-k][j-k]) return 0;
	for(k=1;i>=k;k++)
		if(a[i-k][j]) return 0;
	for(k=1;i>=k&&j+k<N;k++)
		if(a[i-k][j+k]) return 0;
	return 1;
}

int w=0;
void Trial(int i,int n)
{
	int k,m,j;
	if(i==n)
	{
		printf("八皇后\n");
		for(k=0;k<n;k++)
		{
			for(m=0;m<n;m++)
			{
				printf("%4d",a[k][m]);
			}
			printf("\n");
		}
		w++;
		//exit(0);
	}
	else
	{
		for(j=0;j<n;j++)
		{
			a[i][j]=1;
			if(Valid(i,j))
			{
				//printf("i=%d,j=%d\n",i,j);
				//printf("\n");
				Trial(i+1,n);
			}
			a[i][j]=0;
		}
	}
}

int main()
{
	int i,j;
	Trial(0,N);
	printf("一共有%d种",w);
}
