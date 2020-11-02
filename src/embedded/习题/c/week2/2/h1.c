/*----------------------------------------
**	1. 输入三个数，按由大到小的顺序输出，用
**	指针方法处理
**----------------------------------------
*/

#include <stdio.h>
void max(int *p, int *q)
{
	int t;
	if (*q > *p)
	{
		t = *p;
		*p = *q;
		*q = t;
	}
}
void main()
{
	int a, b, c, *p1, *p2, *p3;
	printf("input 3 numbers:\n");
	scanf("%d%d%d",&a,&b,&c);
	p1 = &a;
	p2 = &b;
	p3 = &c;
	max(p1,p2);
	max(p1,p3);
	max(p2,p3);
	printf("%d,%d,%d",a,b,c);
}
