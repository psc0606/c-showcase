/*----------------------------------------
**	1. 输入两个整数，求它们相除的余数，用
**	带参的宏来编程实现。
**----------------------------------------
*/

#include <stdio.h>
#define S(x,y) (x)%(y)

void main()
{
	int a,b;
	printf("input numbers:\n");
	scanf("%d%d",&a,&b);
	printf("The answer is %d\n",S(a,b));
}
