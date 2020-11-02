#include<stdio.h>
int main()
{
	int n,i,m=1,*p,k=0;
	int flag=0,flag2=1;
	int j=1;
	printf("please input n:");
	scanf("%d",&n);
	int a[n],b[n];
	for(i=0;i<n;i++)
	{
		a[i]=m;
		m++;
	}
	while(flag<n)
	{
		for(p=a;p<a+n;p++)
		{
			if(*p==0)
			{
				
				continue;
			}
			else
			{
				k++;
			}
			if(k==flag2)
			{
				//printf("%-3d",b[*p]);
				b[*p-1]=j++;
				flag2++;
				*p=0;
				flag++;
				k=0;
			}
		}
		
	}
	for(p=b;p<b+n;p++)
	{
		printf("%3d ",*p);
	}
}
