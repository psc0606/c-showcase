/*----------------------------------------
**	4.用指向指针的指针的方法对n个整数排序并
**	输出，要求将排序单独写成一个函数，n个整
**	数在主函数中输入，最后在主函数中输出。
**----------------------------------------
*/

#include <stdio.h>
void fun(int *p,int n)
{
	int i, j, k;
	for (i = 0; i < n -1; i++)
	{
		for (j = 0; j < n-i; j++)
		{
			if (*(p+j)<*(p+j+1))
			{
				k = *(p+j);
				*(p+j) = *(p+j+1);
				*(p+j+1) = k;
			}
		}
	}
}

void main()
{
	int i, n, *p,**q;
	printf("input n = ");
	scanf("%d",&n);
	int a[n];
	p = a;
	q = &p;
	for (i = 0; i < n; i++)
	{
		scanf("%d",*q+i);
	}
	fun(*q,n);
	for (i = 0; i < n; i++)
	{
		printf("%4d",*(*q+i));
	}
	printf("\n");
}
