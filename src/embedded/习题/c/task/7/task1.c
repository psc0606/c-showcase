#include<stdio.h>
#include<string.h>
int main()
{
	/*int a[10];
	int i,j,t;
	for(i=0;i<10;i++)
	{
		scanf("%d",&a[i]);
	}
	for(i=0;i<9;i++)
	{
		for(j=0;j<9-i;j++)
		{
			if(a[j]>a[j+1])
			{
				t=a[j];
				a[j]=a[j+1];
				a[j+1]=t;
			}		
		}
	}
	printf("the sorted numbers:");
	for(i=0;i<10;i++)
	{
		printf("%d\t",a[i]);
	}*/
	char a[10][100],b[100];
	int i,j;
	for(i=0;i<10;i++)
	{
		scanf("%s",a[i]);
	}
	for(i=0;i<9;i++)
	{
		for(j=0;j<9-i;j++)
		{
			if(strcmp(a[j],a[j+1])>0)
			{
				strcpy(b,a[j]);
				strcpy(a[j],a[j+1]);
				strcpy(a[j+1],b);
			}
		}
	}
	printf("the sorted strings:");
	for(i=0;i<10;i++)
	{
		printf("%s\n",a[i]);
	}
}
