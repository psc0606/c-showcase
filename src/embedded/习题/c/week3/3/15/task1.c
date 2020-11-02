#include<stdio.h>
#include<string.h>
int main()
{
	char a[20][100],str[100];
	int n,i,j;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%s",a[i]);
	}
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-1-i;j++)
		{
			if(strcmp(a[j],a[j+1])>0)
			{
				strcpy(str,a[j]);
				strcpy(a[j],a[j+1]);
				strcpy(a[j+1],str);
			}
		}
	}
	printf("the sorted strings:\n");
	for(i=0;i<n;i++)
	{
		printf("%s\n",a[i]);
	}
}
