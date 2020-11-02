#include<stdio.h>
int main()
{
	int m,n,i;
	int a[100];
	int *p,*q;
	printf("please input m,n:");
	scanf("%d,%d",&m,&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	q=a;
	for(p=a;p<a+n;p++)
	{
		if(p<q+m)
		{
			q=p+n-m;
			printf("%d",*q);
		}
		else
			printf("%d",*(p-m));
	}
}



