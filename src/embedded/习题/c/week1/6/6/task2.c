#include<stdio.h>
int main()
{
	int day=9;
	int x1,x2=1;
	while(day>0)
	{
		x1=(x2+1)*2;
		x2=x1;
		day--;
	}
	printf("%d",x1);
}
