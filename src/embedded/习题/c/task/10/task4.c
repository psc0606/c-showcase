#include<stdio.h>
int main()
{
	int a[100],n,i,m=1,k=1,flag=0;
	int *p;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		a[i]=m++;
	}
	while(flag<n-1)
	{
		for(p=a;p<a+n;p++)
		{
			printf("%d\t",*p);
			if(*p==0)
			{
				continue;
			}
			if(k==3)
			{
				flag++;
				//printf("flag=%d\n",flag);
				*p=0;
				k=1;
			}
			else
			{
				k++;
			}
		}
		printf("\n");
	}
	//printf("flag=%d\n",flag);
	for(p=a;p<a+n;p++)
	{
		printf("%d\t",*p);
	}
}
