/*3.	编写函数int palindrome(char string[]) 如果参数字符串是个回文就返回1，否则返回0。*/
#include<stdio.h>
#include<string.h>
int palindrome(char *string)
{
	int length;
	int i,j,k,flag=0;
	length=strlen(string);
	for(i=0,j=length-1;i<length/2;i++,j--)
	{
		if(string[i]==string[j])
		{
			flag++;
		}
	}
	if(flag==length/2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int main()
{
	char string[20];
	int result;
	printf("input the string:");
	scanf("%s",string);
	result=palindrome(string);
	switch(result)
	{
		case 1:printf("1 was returned.\n");break;
		case 0:printf("0 was returned.\n");break;
	}
	return 0;
}
