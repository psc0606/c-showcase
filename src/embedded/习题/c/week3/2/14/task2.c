#include<stdio.h>
#define MFLAG 1
int main()
{
	int i,m;
	int a[10];
	for(i=0;i<10;i++)
	{
		scanf("%d",&a[i]);
	}
	m=a[0];
	for(i=0;i<10;i++)
	{
		#if MFLAG
		
		if(m<a[i])
			m=a[i];
		#else
			if(m>a[i])
				m=a[i];
		
		#endif
	}
	printf("m=%d",m);	
}
