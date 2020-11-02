#include<stdio.h>
int main()
{
	int m,n,a[100],i=0;
	printf("please input m,n:");
	scanf("%d,%d",&m,&n);
	int *p,*q;
	for(p=a;p<a+n;p++)
	{
		scanf("%d",p);
	}
	q=a;
	for(p=a;p<a+n;p++)
	{
		if(p<q+m)
		{
			q=a+n-1-i;
			printf("%d",*q);
			q=a;
		}
		else
		{
			printf("%d",*(p-m));
		}
		i++;
	}
}
