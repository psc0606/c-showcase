/*----------------------------------------
**	4.有n个人围成一圈，顺序排号。从第1个人开始
**	报数（从1 到3报数），凡报到3的人退出圈子，
**	试打印出这n个人退出的次序。
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j = 0, k = 0, n;
	printf("input n = ");
	scanf("%d",&n);
	int a[n+1];
	for (i = 0; i <= n; i++)
	{
		a[i] = i;
	}
	for (i = 1; ; i++)
	{
		if (i > n)
		{
			i = 1;
		}
		if (a[i] != 0)
		{
			j++;
			if (j % 3 == 0)
			{
				a[i] = 0;
				k++;
				printf("%4d",i);
			}
		}
		if (k == n)
		{
			printf("\nthe last is %d\n",i);
			break;
		}
	}
}
