#include<stdio.h>
int main()
{
	int a[10]={10,2,8,22,16,4,10,6,14,20};
	int *p[10];
	int i,n=0;
	p[0]=a;
	while(1)
	{
		/*if(*p[0]==*p[1]==*p[2]==*p[3]==*p[4]==*p[5]==*p[6]==*p[7]==*p[8]==*p[9])
			break;*/
		for(i=0;i<10;i++)
		{
			
		}
		else
		{
			for(i=0;i<10;i++)
			{
				if(*p[i]%2==0)
					*p[(i+1)%10]=*p[i]/2+*p[(i+1)%10];
				else
					*p[(i+1)%10]=(*p[i]+1)/2+*p[(i+1)%10];
			}
			n++;
		}
	}
	printf("*p[0]=%d",*p[0]);
	printf("n=%d",n);
}
