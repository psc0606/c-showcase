#include<stdio.h>
int main()
{
	char str[100];
	gets(str);
	int i,upper=0,lower=0,word=0,flag=0;
	for(i=0;str[i]!='\0';i++)
	{
		if(str[i]>=65 && str[i]<=90)
			upper++;
		if(str[i]>=97 && str[i]<=122)
			lower++;
		if(str[i]==' ')
			flag=0;
		else if(flag==0)
		{
			flag=1;
			word++;
		}
	}
	printf("大写字母：%d,小写字母:%d,单词：%d",upper,lower,word);
}
