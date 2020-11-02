/*----------------------------------------
**	2. 计算任意输入两个数的和与积。
**	（用函数与指针写）
**----------------------------------------
*/

#include <stdio.h>

float sum(double a,double b)
{
	return a + b;
}

float mul(double a,double b)
{
	return a * b;
}

void main()
{
	float a, b, (*p)(), (*q)();
	p = sum;
	q = mul;
	printf("input numbers:\n");
	scanf("%f%f",&a,&b);
	printf("sum = %8.2f\nmul = %8.2f\n"
			,p(a,b),q(a,b));
}
