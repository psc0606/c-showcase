#include<stdio.h>
int main()
{
	double a[10],min,max,aver,s=0;
	int i;	
	for(i=0;i<10;i++)
	{
		scanf("%lf",&a[i]);
	}
	max=min=a[0];
	for(i=0;i<10;i++)
	{
		if(a[i]<min)
			min=a[i];
		if(a[i]>max)
			max=a[i];
	}
	for(i=0;i<10;i++)
	{
		s+=a[i];
	}
	aver=(s-min-max)/8;
	printf("%lf",aver);
}
