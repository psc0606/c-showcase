/*----------------------------------------
**	1. 一个数组的最长递减序列，比如
**	{9,4,3,2,5,4,3,2}的最长递减序列为
**	{9,5,4,3,2}；即将数组中的重复元素去掉，
**	然后将数组排序。
**----------------------------------------
*/

#include <stdio.h>
int fun(int *p, int n)
{
	int i, j, k, t, m = 0;
	for (i = 0; i < n - m; i++)
	{
		for (j = 0; j < n-m-i; j++)
		{
			if (*(p+j)<*(p+j+1))
			{
				t = *(p+j);
				*(p+j) = *(p+j+1);
				*(p+j+1) = t;
			}
		}
	}
	
	
	for (i = 0; i < n-m-1; i++)
	{
		if (*(p+i) == *(p+i+1))
		{
			m++;
			k = i;
			while(k < n-m-1)
			{
				*(p+k) = *(p+k+1);
				k++;
			}
			i--;
		}
	}
	return m;
}

void main()
{
	int i, j, k, m, n, a[20], *p;
	p = a;
	printf("input n = ");
	scanf("%d",&n);
	printf("input value\n");
	for (i = 0; i < n; i++)
	{
		scanf("%d",p+i);
	}
	m = fun(p,n);
	for (i = 0; i < n - m; i++)
	{
		printf("%4d",*(p+i));
	}
}
