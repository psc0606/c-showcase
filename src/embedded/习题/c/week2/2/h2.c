/*----------------------------------------
**	2. 有n个整数，使前面各数顺序向后m个位置，
**	最后m个数变成最前m个数。写一个函数实现以
**	上功能，在主函数中输入n个整数和输出调整后
**	的n个整数。（用指针实现）
**----------------------------------------
*/

#include <stdio.h>

void swap(int *a,int n,int m)
{
	int i, b[m];
	for (i = 0; i < m; i++)
	{
		b[i] = *(a+n-m+i);
	}
	for (i = --n; i >= m; i--)
	{
		*(a+i) = *(a+i-m);
	}
	for (i = 0; i < m; i++)
	{
		*(a+i) = b[i];
	}
}

void main()
{
	int i, n, m;
	printf("input n = ");
	scanf("%d",&n);
	int a[n];
	printf("input numbers\n");
	for (i = 0; i < n; i++)
	{
		scanf("%d",&a[i]);
	}
	printf("input m = ");
	scanf("%d",&m);
	swap(a,n,m);
	for (i = 0; i < n; i++)
	{
		printf("%3d",a[i]);
	}
	printf("\n");
}
