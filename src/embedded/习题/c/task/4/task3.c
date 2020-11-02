#include<stdio.h>
int main()
{
	int n,a,b,c,d,m;
	for(n=1000;n<=9999;n++)
	{
		a=n%1000%100%10;
		b=n%1000%100/10;
		c=n%1000/100;
		d=n/1000;
		m=a*1000+b*100+c*10+d;
		if(m==9*n)
		printf("%d\n",n);
	}
}
