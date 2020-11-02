#include<stdio.h>
#include<string.h>
int main()
{
	char str[100],str1[100],str2[100];
	printf("请输入一句英文:");
	gets(str);
	printf("请输入要替换的字符串:");
	gets(str1);
	printf("替换成:");
	gets(str2);
	char *p,*q;
	int n=0;
	for(p=str;*p!='\0';p++)
	{
		printf("*p=%c\n",*p);
		for(q=str1;*q!='\0';q++)
		{
			if(*p==*q)
			{
				p++;
				n++;	
			}
			else
			{
				p=p-n;
				n=0;
				break;				
			}
		}
		if(n==strlen(str1))
		{
			p=p-n;
			n=0;
			for(q=str2;*q!='\0';q++)
			{
				*p=*q;
				p++;
			}
			p--;
		}
	}
	printf("%s",str);
}
