#include<stdio.h>

void jiami(char *p)
{
	for(;*p!='\0';p++)
	{
		if(*p>=65 && *p<=90)
		{
			*p=(*p-65+4)%26+65+32;
		}
		else if(*p>=97 && *p<=122)
		{
			*p=(*p-97+4)%26+97-32;
		}
	}
}

void jiemi(char *p)
{
	for(;*p!='\0';p++)
	{
		if(*p>=65 && *p<=90)
		{
			*p=(*p-65-4)%26+65+32;
		}
		else if(*p>=97 && *p<=122)
		{
			*p=(*p-97-4)%26+97-32;
		}
	}
}

int main()
{
	printf("*************************************\n");
	printf("1:加密\n");
	printf("2:解密\n");
	printf("3:退出\n");
	printf("*************************************\n");
	char str[100];
	int m;
	printf("please input the string\n");
	gets(str);
	while(printf("please input your choose:"),scanf("%d",&m),m!=3)
	{
		if(m==1)
		{
			jiami(str);
			printf("%s\n",str);
		}
		if(m==2)
		{
			jiemi(str);
			printf("%s\n",str);
		}
	}
}
