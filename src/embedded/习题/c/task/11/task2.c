#include<stdio.h>
#include<string.h>
int main()
{
	char str1[100],str2[100],str3[100];
	memset(str3,'\0',100);
	gets(str1);
	gets(str2);
	int i=0;
	char *p;
	for(p=str1;*p!='\0';p++)
	{
		str3[i++]=*p;
	}
	for(p=str2;*p!='\0';p++)
	{
		str3[i++]=*p;
	}
	printf("%s",str3);
}
