#include<stdio.h>

void input(int *p)
{
	int i;
	for(i=0;i<10;i++)
	{
		scanf("%d",p++);
	}	
}

void change(int *a)
{
	int *p,*q;
	int t;
	q=a;
	for(p=a;p<a+10;p++)
	{
		if(*p<*q)
			q=p;
	}
	t=*q;
	*q=a[0];
	a[0]=t;
}

void output(int *p)
{
	int i;
	for(i=0;i<10;i++)
	{
		printf("%d\t",*p);
		p++;
	}
}

int main()
{
	int a[10];
	printf("请输入十个数:");
	input(a);
	change(a);
	output(a);
}
