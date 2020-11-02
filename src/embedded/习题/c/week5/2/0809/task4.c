#include<stdio.h>
#include<string.h>
#include<malloc.h>
int main()
{
	int n,i=0,j=0;
	printf("please input n:");
	scanf("%d",&n);
	char *p[10],str[100];
	for(i=0;i<n;i++)
	{
		p[i]=(char*)malloc(20*sizeof(char));
	}
	for(i=0;i<n;i++)
	{
		scanf("%s",p[i]);
	}
	for(i=1;i<n;i++)
	{
		strcpy(str,p[i]);
		for(j=i-1;j>=0;j--)
		{
			if(strcmp(p[j],str)>0)
				strcpy(p[j+1],p[j]);	
			else
				break;	
		}
		strcpy(p[j+1],str);
	}
	printf("the sorted string:\n");
	for(i=0;i<n;i++)
	{
		printf("%s\n",p[i]);
	}
}
