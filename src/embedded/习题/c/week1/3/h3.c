/*----------------------------------------
** A 、B、 C、 D、 E五人在某天夜里合伙去捕鱼
**	A第一个醒来，他将鱼分为五份，把多余的一条
**	鱼扔掉，拿走自己的一份,BCDE同样，求鱼数
**----------------------------------------
*/

#include <stdio.h>
int f(int a)
{	
	int b;
	b = (a - 1);
	if (b % 5 == 0)
	{
		b = b * 4/5;
		return b;
	}
	else 
	return 0;
}
void main()
{
	int A,B,C,D,E,F,G=0;
	for (A=1;!G;A++)
	{
		B = f(A);
		C = f(B);
		D = f(C);
		E = f(D);
		F = f(E);
		G=B&&C&&D&&E&&F;
	}
	printf("%d\n",--A);
}	
