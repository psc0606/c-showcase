/*----------------------------------------
**	5. 求2个数的最大公约数和最小公倍数。
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j, x, y;
	printf("请输入2个数:\n");
	scanf("%d%d",&x,&y);
	i = (x > y)?y:x;
	for (i; i > 0; i--)
	{
		if ( x % i == 0 && y % i == 0)
		{
			printf("最大公约数为:%d\n",i);
			break;
		}
	}
	for (i = 1; i <= x * y; i++)
	{
		if (i % x == 0 && i % y == 0)
		{
			printf("最小公倍数为:%d\n",i);
			break;
		}
	}
}
