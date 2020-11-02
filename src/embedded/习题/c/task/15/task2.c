#include<stdio.h>
#include<string.h>
/*int main()
{
	int a[100];
	int n,i,j,k,t;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	for(i=0;i<n-1;i++)
	{
		k=i;
		for(j=i+1;j<n;j++)
		{
			if(a[j]<a[k])
				k=j;
		}
		t=a[k];a[k]=a[i];a[i]=t;
	}
	printf("the sorted numbers:\n");
	for(i=0;i<n;i++)
	{
		printf("%d\t",a[i]);
	}
}*/

int main()
{
	char a[20][100],str[100];
	int n,i,j,k,t;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%s",a[i]);
	}
	for(i=0;i<n-1;i++)
	{
		k=i;
		for(j=i+1;j<n;j++)
		{
			if(strcmp(a[j],a[k])>0)
				k=j;
		}
		strcpy(str,a[k]);
		strcpy(a[k],a[i]);
		strcpy(a[i],str);
	}
	printf("the sorted strings:\n");
	for(i=0;i<n;i++)
	{
		printf("%s\n",a[i]);
	}
}
