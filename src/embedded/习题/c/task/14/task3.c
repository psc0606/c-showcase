#include<stdio.h>
#include<malloc.h>
#include<string.h>

int fun(void *x,void *y)
{
	int * p1 = (int*)x;
	int* p2 = (int*)y;
	if(*p1==*p2)
		return 0;
	if(*p1<*p2)
		return -1;
	if(*p1>*p2)
		return 1;
}
int fun1(void *x,void *y)
{
	char * p1 = (char*)x;
	char* p2 = (char*)y;
	if(*p1==*p2)
		return 0;
	if(*p1<*p2)
		return -1;
	if(*p1>*p2)
		return 1;
}


void sort(void *p,int n,int m,int (*fun)(void *,void *))
{
	int i,j;
	char*  p1 = (char*)p;
	char*  p2 = (char*)p+m;
	
	char* t = malloc(m);
	for(i=0;i<n-1;i++)
	{
		p1 =(char*)p;
		p2 = (char*)p + m;
		for(j=0;j<n-1-i;j++)
		{
			if( fun(p1,p2)<0)
			{
				memcpy(t,p1,m);
				memcpy(p1,p2,m);
				memcpy(p2,t,m);
			}
			p1 = p1+m;
			p2 = p2+m;
		}
	}
}

int main()
{
	int a[10];
	//char b[10]={"abcdeADFD"};
	int i;
	for(i=0;i<10;i++)
		scanf("%d",a+i);
	sort(a,10,sizeof(int),fun);
	for(i=0;i<10;i++)
		printf("%d\t",a[i]);
}
