/*----------------------------------------
**	 4、 在屏幕上显示杨辉三角，行数由键盘输入 
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i,j,k,n,a[100][100];
	printf("请输入行数:\n");
	scanf("%d",&n);
	for (i = 0; i < n; i++)
	{
		a[i][0] = 1;
		a[i][i] = 1;
	}
	for (j = 1; j < n; j++)
	{
		for (k = 1; k < j; k++)
		{
			a[j][k] = a[j-1][k-1] + a[j-1][k];
		}
	}
	for (j = 0; j < n; j++)
	{
		for (i = 1; i < n-j; i++)
		{
			printf("  ");
		}
		for (k = 0; k <= j; k++)
		{
			printf("%d   ",a[j][k]);
			if(j == k)
			{
				printf("\n");
			}
		}

	}
}
