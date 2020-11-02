#include<stdio.h>
#include<string.h>
void reverse_string(char* s)
{
	char *p,*q;
	char temp;
	int n=strlen(s);
	q=s+n-1;
	for(p=s;p<s+(n+1)/2;p++)
	{
		//printf("*q=%c\n",*q);
		temp=*p;
		*p=*q;
		*q=temp;
		q--;
	}
}

int main()
{
	char str[100];
	gets(str);
	reverse_string(str);
	printf("%s",str);
}
