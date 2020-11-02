#include<stdio.h>
#include<string.h>
void deblank(char* p)
{
	char str1[100];
	char b[100];
	memset(b,'\0',100);
	int flag=0,i=0;
	for(;*p!='\0';p++)
	{
		if(flag==0 && *p==' ')
		{
			b[i++]=*p;
		}
		if(*p!=' ')
		{
			b[i++]=*p;
			flag=0;
		}
		if(*p==' ')
		{
			flag=1;
		}
	}
	printf("%s",b);
}

int main()
{
	char str[100];
	gets(str);
	deblank(str);
}
