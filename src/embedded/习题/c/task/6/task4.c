#include<stdio.h>
int main()
{
	int x,y,z,n=0;
	for(x=1;x<30;x++)
	{
		for(y=1;y<30;y++)
		{
			for(z=1;z<30;z++)
			{
				if(x+y+z==30 && 3*x+2*y+z==50)
				{
					n++;
					printf("%d,%d,%d\n",x,y,z);
				}
			}
		}
	}
	printf("一共有%d种",n);
}
