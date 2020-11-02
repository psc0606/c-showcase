#include<stdio.h>
void main()
{
	float fac(int n);
	float y;
	y=fac(10)/(fac(3)*fac(10-3));
	printf("%10.0f\n",y);
}

float fac(int n)
{
	float f;
	if(n<0)
	{
		printf("n<0,dataerror!");
	}
	else
		if(n==0||n==1)
		f=1;
	else
		f=fac(n-1)*n;
	return(f);
}

