/*----------------------------------------
**	1. 利用malloc和双指针实现螺旋矩阵。
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>

void main()
{
	int i, j, k, n, m = 1, **p;
	printf("input n = ");
	scanf("%d",&n);
	p = (int **)malloc(n*sizeof(int *));
	for (i = 0; i < n; i++)
	{
		p[i] = (int *)malloc(n*sizeof(int));
	}
	
	for (k = 0; k < (n+1)/2; k++)
	{
		for (i = k, j = k; j < n-k; j++)
		{
			p[i][j] = m++;
		}
		for (j--,i++; i < n-k; i++)
		{
			p[i][j] = m++;
		}
		for (i--,j--; j >= k; j--)
		{
			p[i][j] = m++;
		}
		for (j++,i--; i > k; i--)
		{
			p[i][j] = m++;
		}
		
	}
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%4d",p[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0; i < n; i++)
	{
		free(p[i]);
	}
	free(p);
}
