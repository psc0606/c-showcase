/*----------------------------------------
**	1. 使用指针完成对一维数组a的以下操作 
**	1重新赋值 2计算数组的平均值 
**	3按升序将数组元素输出。
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j, k, n, *p;
	double sum = 0;
	printf("input n = ");
	scanf("%d",&n);
	int a[n];
	p = a;
	printf("input value\n");
	for (i = 0; i < n; i++)
	{
		scanf("%d",p+i);
		sum += *(p + i);
	}
	printf("average = %f\n",	sum/n);
	for (i = 0; i < n-1; i++)
	{
		for (j = 0; j < n-1-i; j++)
		{
			if (*(p + j) > *(p + j +1 ))
			{
				k = *(p + j +1 );
				*(p + j +1 ) = *(p + j);
				*(p + j) = k;
			}
		}
	}
	
	for (i = 0; i < n; i++)
	{
		printf("%4d",*(p+i));
	}
	printf("\n");
}
