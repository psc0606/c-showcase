/*----------------------------------------
**	5.作出任意阶数的回型数组，5阶示例如下：
**	1  1   1   1   1   1   1   1   1
**	1  2   2   2   2   2   2   2   1
**	1  2   3   3   3   3   3   2   1
**	1  2   3   4   4   4   3   2   1
**	1  2   3   4   5   4   3   2   1
**	1  2   3   4   4   4   3   2   1
**	1  2   3   3   3   3   3   2   1
**	1  2   2   2   2   2   2   2   1
**	1  1   1   1   1   1   1   1   1
**----------------------------------------
*/
/*
#include <stdio.h>
void main()
{
	int i, j, k, n, m = 0;
	printf("input n = ");
	scanf("%d",&n);
	int a[2*n-1][2*n-1];

	for (i = 0; i < n; i++)
	{
		for (j = i; j < n; j++)
		{
			a[i][j] = (i+1);
			a[j][i] = (i+1);
		}
	}
	
	for (i = 0; i < 2*n-1; i++)
	{
		for (j = 0; j < 2*n-1; j++)
		{
			if (i >= n && j >= n)
			{
				printf("%3d",a[2*n-2-i][2*n-2-j]);
			}
			else if (i >= n)
			{
				printf("%3d",a[2*n-2-i][j]);
			}
			else if (j >= n)
			{
				printf("%3d",a[i][2*n-2-j]);
			}
			else
			{
				printf("%3d",a[i][j]);
			}
		}
		printf("\n");
	}
}
*/

#include <stdio.h>
void main()
{
	int i, j, k, n, m = 0;
	printf("input n = ");
	scanf("%d",&n);
	int a[2*n-1][2*n-1];

	for (i = 0; i < n; i++)
	{
		for (j = i; j < 2*n-1-i; j++)
		{
			a[i][j] = (i+1);
			a[2*n-2-i][j] = (i+1);
			a[j][i] = (i+1);
			a[j][2*n-2-i] = (i+1);
		}
	}
	
	
	
	for (i = 0; i < 2*n-1; i++)
	{
		for (j = 0; j < 2*n-1; j++)
		{
			printf("%3d",a[i][j]);
		}
		printf("\n");
	}
}

