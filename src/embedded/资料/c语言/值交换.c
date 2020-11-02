#include <stdio.h>
void main()
{
	int a, b, *p, *q;
	scanf("%d%d",&a,&b);
	p = &a;
	q = &b;
	void max();
	max(p,q);
	printf("a = %d, b = %d",a,b);
}

void max(int *x, int *y)
{
	int z;
	if (*x < *y)
	{
		z = *y;
		*y = *x;
		*x = z;
	}
}
