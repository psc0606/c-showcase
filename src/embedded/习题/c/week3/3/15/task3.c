#include<stdio.h>
#include<string.h>
/*int main()
{
	int a[100],i=0,j=0,temp=0,n;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	for(i=1;i<n;i++)
	{
		temp=a[i];
		for(j=i-1;j>=0;j--)
		{
			if(a[j]>temp)
			{
				a[j+1]=a[j];
			}
			else
			{
				break;
			}
		}
		a[j+1]=temp;
	}
	printf("the sorted numbers:\n");
	for(i=0;i<n;i++)
	{
		printf("%d\t",a[i]);
	}
}*/

int main()
{
	char name[20][100],tel[20][100],temp[100],temp1[100];
	int n,i=0,j=0;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%s%s",name[i],tel[i]);
	}
	for(i=1;i<n;i++)
	{
		strcpy(temp,name[i]);
		strcpy(temp1,tel[i]);
		for(j=i-1;j>=0;j--)
		{
			if(strcmp(name[j],temp)>0)
			{
				strcpy(name[j+1],name[j]);
				
				strcpy(tel[j+1],tel[j]);
			}
			else
			{
				break;
			}
		}
		strcpy(name[j+1],temp);
		strcpy(tel[j+1],temp1);
	}
	printf("the sorted:\n");
	for(i=0;i<n;i++)
	{
		printf("%s\t%s\n",name[i],tel[i]);
	}
}
