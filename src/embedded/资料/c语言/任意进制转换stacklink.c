#include <stdio.h>
#include <malloc.h>
#include "stack_link.h"

void main()
{
	int a, b, n, m;
	printf("input H = ");
	scanf("%d",&a);
	printf("input B = ");
	scanf("%d",&b);
	stack *s;
	initstack(&s);
	for (;a != 0; a = a / b)
	{
		n = a % b;
		push(&s,n);
	}
	while (!getTop(&s,&m))
	{
		printf("%d",m);
		pop(&s,&m);
	}
}
