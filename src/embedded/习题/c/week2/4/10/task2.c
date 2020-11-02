#include<stdio.h>

char *q;
int string_cut(char *str,char c)
{
	char *p;
	for(p=str;*p!='\0';p++)
	{
		if(*p==c)
		{
			q=p;
			return 1;
		}
	}
	return 0;
}
int main()
{
	char str[100];
	char c;
	printf("please input the string\n");
	gets(str);
	printf("please input c\n");
	scanf("%c",&c);
	if(string_cut(str,c))
		printf("%s\n",q);
	else
		printf("the character isn't include.");
}
