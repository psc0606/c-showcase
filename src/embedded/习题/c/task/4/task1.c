#include<stdio.h>
int main()
{
	int n,k,m,s=0;
	for(n=100;n<1000;n++)
	{
		k=n;
		while(k!=0)
		{
			m=k%10;
			s+=m*m*m;
			k=k/10;
		}
		if(s==n)
			printf("%d\n",n);
		s=0;
	}
}
