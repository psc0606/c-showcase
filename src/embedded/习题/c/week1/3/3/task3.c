#include<stdio.h>
int main()
{
	long int n=6,x;
	int i;
	while(1)
	{
		n++;
		for(x=n,i=0;i<5;i++)
		{
			if((x-1)%5==0)
				x=4*(x-1)/5;
			else
			{
				break;
			}
		}
		if(i==5)
		{
			printf("%ld",n);
			break;
		}
	}
}
