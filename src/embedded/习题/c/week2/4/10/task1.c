#include<stdio.h>
#include<math.h>
int main()
{
	long long int x,y,m,s,k;
	int n;
	for(x=0;x<=200000;x++)
	{
		k=x;
		y=k*k;
		s=0;
		n=0;
		while(k!=0)
		{
			m=y%10;
			n++;
			s+=m*(int)pow(10,n-1);
			y=y/10;
			k/=10;
		}
		if(x==s)
			printf("%lld\n",x);
	}
}


