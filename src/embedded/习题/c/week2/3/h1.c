/*----------------------------------------
**	1. 输入10个整数，将其中最小的数与第一个数对换
**要求写三个函数：（1）输入十个整数（2）进行处理
**（3）输出十个数（4）上面三个函数需要用指针进行处理
**----------------------------------------
*/

#include <stdio.h>

void input(int *p)
{
	int i;
	for (i = 0; i < 10; i++)
	{
		scanf("%d",p+i);
	}
}

void swap(int *p)
{
	int i, j, k, t;
	for (i = 0,j = 0; i < 10; i++)
	{
		k = i;
		if (*(p+j)>*(p+k))
		{
			j = k;
		}
	}
	t = *(p+j);
	*(p+j) = *p;
	*p = t;
}

void output(int *p)
{
	int i;
	for (i = 0; i < 10; i++)
	{
		printf("%3d",*(p+i));
	}
	printf("\n");
}

void main()
{
	int a[10];
	input(a);
	swap(a);
	output(a);	
}
