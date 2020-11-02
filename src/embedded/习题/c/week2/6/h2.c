/*----------------------------------------
**	2.求1到n共n个数中取出m个不同的数（m<=n），
**	共有多少种组合方式。算法：使用数学中的组合
**	公式，C=n!/[(n-m)!m!]
**----------------------------------------
*/

#include <stdio.h>
int fun(int n)
{
	int f;
	if (n == 1)
	{
		f = 1;
	}
	else
	{
		f = n * fun(n-1);
	}
	return f;
}

void main()
{
	int m, n;
	printf("input n = ");
	scanf("%d",&n);
	printf("input m = ");
	scanf("%d",&m);
	printf("C = n!/[(n-m)!m!] = %d/[%d*%d] = %d\n",fun(n),fun(n-m),fun(m),fun(n)/(fun(n-m)*fun(m)));
}
