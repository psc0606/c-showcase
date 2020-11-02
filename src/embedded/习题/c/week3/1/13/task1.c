#include<stdio.h>
#include<string.h>
int main()
{
	char str[100],a[10][100];
	char *p,*q;
	int i=0,j=0,flag=0,n=1,m;
	memset(str,'\0',100);
	gets(str);
	for(p=str;*p!='\0';p++)
	{
		if(*p!=' ')
		{
			flag=0;
			a[i][j]=*p;
			j++;
		}
		else
		{
			if(flag==0)
			{
				n++;
				i++;
				flag=1;
			}
			a[i][j]='\0';
			j=0;
		}	
	}
	for(i=0;i<3;i++)
	{
		printf("%s\n",a[i]);
	}
	printf("n=%d",n);
	m=strlen(a[0]);
	for(i=0;i<n;i++)
	{
		if(strlen(a[i])>m)
			q=a[i];
	}
	printf("最长单词=%s\n",q);
}


