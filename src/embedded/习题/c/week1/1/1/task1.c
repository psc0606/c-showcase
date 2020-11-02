#include<stdio.h>
int main()
{
	char str[100];
	int i;
	printf("请输入一行英文\n");
	gets(str);
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]>=97 && str[i]<=122)
		{
			str[i]-=32;
		}
	}
	printf("the sorted words:\n");
	for(i=0;str[i]!='\0';i++)
	{
		printf("%c",str[i]);
	}
	printf("\n");
}
