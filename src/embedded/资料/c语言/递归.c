#include <stdio.h>
int fun(int n)
{
	if (1 == n || 2 == n)
	{
		return 1;
	}
	else
	{
		return fun(n-1)+fun(n-2);
	}
	
}
void main()
{
	int n;
	scanf("%d",&n);
	printf("%d\n",fun(n));
}
