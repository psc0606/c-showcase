#include<stdio.h>
int main()
{
	int a,b,m,n;
	scanf("%d/%d,%d/%d",&a,&b,&m,&n);
	if(a*n>m*b)
		printf("%d/%d is biger",a,b);
	else
		printf("%d/%d is biger",m,n);
}
