#include<stdio.h>

int change(char *str)
{
	int count=0,i,j;
	for(i=0,j=0;str[i]!='\0';i++)
	{
		if(str[i]=='*')
		{
			for(j=i-1;str[j]!='*'&&j>=0;j--)
			{
				str[j+1]=str[j];
			}
			str[j+1]='*';
			count++;
		}
	}
	return count;
}
int main()
{
	char str[]="ab**cd**e*12";
	printf("str1=%s\n",str);
	printf("str2=%s,count=%d",str,change(str));
}
