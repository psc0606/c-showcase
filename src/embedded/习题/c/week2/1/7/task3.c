#include<stdio.h>
#include<string.h>
int palindrome(char str[100])
{
	int i,j=strlen(str)-1;
	for(i=0;i<=strlen(str)/2;i++)
	{
		if(str[i]==str[j])
		{
			j--;
			continue;
		}
		else
			break;
	}
	if(i==strlen(str)/2+1)
		return 1;
	else
		return 0;
}
int main()
{
	char a[100];
	gets(a);
	if(palindrome(a))
		printf("是回文");
	else
		printf("不是回文");	
}
