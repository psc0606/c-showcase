/*----------------------------------------
**	5. 用双指针和malloc，实现二维数组的动态
**	分配、输入和输出功能。
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>

void main()
{
	int i, j, k = 1, m, n, **p;
	printf("m x n ,m = ");
	scanf("%d",&m);
	printf("m x n ,n = ");
	scanf("%d",&n);
	
	p = (int **)malloc(m*sizeof(int *));
	
	for (i = 0; i < m; i++)
	{
		p[i] = (int *)malloc(n*sizeof(int));
		for (j = 0; j < n; j++)
		{
			//p[i][j] = k++;
			scanf("%d",&p[i][j]);
			//printf("%4d",p[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%4d",p[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0; i < m; i++)
	{
		free(p[i]);
	}
	free(p);
}

