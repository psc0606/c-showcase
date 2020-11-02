/*----------------------------------------
**	3. 编写一个函数，使给定的一个3*3的二维整形数
**	组转置（即行列互换）
**----------------------------------------
*/

#include <stdio.h>

void main()
{
	int i, j, k,n, l = 1;
	printf("input n x n :\n");
	scanf("%d",&n);
	int a[n][n];
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			a[i][j] = l++;
		}
	}
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%3d",a[i][j]);
		}
		printf("\n");
	}
	
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < i; j++)
		{
			k = a[i][j];
			a[i][j] = a[j][i];
			a[j][i] = k;
		}
	}
	
	printf("\n");
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%3d",a[i][j]);
		}
		printf("\n");
	}
}
