#include<stdio.h>
#define S(a,b) a%b

int main()
{
	int a,b;
	scanf("%d,%d",&a,&b);
	printf("%d",S(a,b));
}
