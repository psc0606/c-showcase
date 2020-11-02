#include<stdio.h>
int main()
{
	/*int m,n,t,r,x;
	scanf("%d,%d",&m,&n);
	x=m*n;
	if(m<n)
	{
		t=m;
		m=n;
		n=t;
	}
	while(m%n!=0)
	{
		r=m%n;
		m=n;
		n=r;
	}
	printf("gongyue:%d\n",n);
	printf("gongbei:%d",x/n);*/
	int i;
	for(i=1;i<100;i++)
	{
		if(i%14==0)
		{
			printf("%d\n",i);
		}
	}
}
