/*----------------------------------------
**	1. 编写程序，实现以下公式	10	(x=0)
**				fun(x) = fun(x-1)+2	(x>0)
**	提示：使用递归或非递归
**----------------------------------------
*/

#include <stdio.h>

int fun(int x)
{
	if (x == 0)
	{
		return 10;
	}
	else
	{
		return fun(x-1) + 2;
	}
}

void main()
{
	int n;
	scanf("%d",&n);
	printf("%d\n",fun(n));
}
