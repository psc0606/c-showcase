#include<stdio.h>

int sum(int x,int y)
{
	return x+y;
}

int product(int x,int y)
{
	return x*y;
}

int anyfunc(int (*pfun)(int x,int y),int x,int y)
{
	return pfun(x,y);	
}

int main()
{
	int a,b;
	int (*pf)(int,int)=sum;
	printf("please input a,b:");
	scanf("%d,%d",&a,&b);
	
	printf("the sum is %d and the ji is %d",anyfunc(pf,a,b),anyfunc(product,a,b));
}
