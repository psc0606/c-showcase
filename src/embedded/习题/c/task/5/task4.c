#include<stdio.h>
int main()
{
	int p,e,a,r;
	for(p=0;p<10;p++)
	{
		for(e=0;e<10;e++)
		{
			for(a=0;a<10;a++)
			{
				for(r=0;r<10;r++)
				{
					if((p*1000+e*100+a*10+r)-(a*100+r*10+a)==(p*100+e*10+a) && !(p==0&&e==0&&a==0&&r==0))
					{
						printf("P=%d,E=%d,A=%d,R=%d\n",p,e,a,r);
					}
				}
			}
		}
	}
}
