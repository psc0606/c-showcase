#include<stdio.h>
int main()
{
	int n,k,i,s=0;
	for(n=1;n<1000;n++)
	{
		k=n;
		for(i=1;i<k;i++)
		{
			if(k%i==0)
				s+=i;
		}
		if(n==s)
			printf("%d\n",n);
		s=0;
	}
}
