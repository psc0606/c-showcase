#include<stdio.h>
int main()
{
	int a[100],i,j,n,temp;
	float sum=0.0;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	for(i=0;i<n;i++)
	{
		sum+=a[i];
	}
	printf("the aver is:%f\n",sum/n);
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-1-i;j++)
		{
			if(a[j]>a[j+1])
			{
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
	}
	printf("the sorted number:\n");
	for(i=0;i<n;i++)
	{
		printf("%d\t",a[i]);
	}
}
