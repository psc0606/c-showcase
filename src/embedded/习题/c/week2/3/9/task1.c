#include<stdio.h>
#include<string.h>
int main()
{
	char str[100];
	gets(str);
	char a[100][100];
	int i=0,j=0,flag=0;
	char *p;
	for(p=str;*p!='\0';p++)
	{
		if(*p>=48 && *p<=57)
		{
			j++;
		}
		if(*p>=48&&*p<=57 && flag==0)
		{
			a[i++]=p;
			flag=1;
			//j++;
		}
		else if(*p<48 || *p>57)
		{
			flag=0;
			a[i][j]='\0';
			//i--;
			//a[i][j]='\0';
			//j=0;
			//a[i--][j]='\0';
			//j=0;
		}
	}
	for(i=0;i<3;i++)
	{
		printf("%s\n",a[i]);
	}
	//printf("%s",a[0]);
}
