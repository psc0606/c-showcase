//1.输入十个字符串，按首字母对其进行排序。
#include<stdio.h>
#include<string.h>
int main()
{
	char string[10][10];
	char strtemp[10];
	int i,j;
	for(i=0;i<10;i++)
	{
		printf("input string[%d]:",i);
		scanf("%s",string[i]);
	}
	for(i=0;i<10;i++)
	{
		for(j=i;j<10;j++)
		{
			if(strcmp(string[i],string[j])>=0)
			{
				strcpy(strtemp,string[i]);
				strcpy(string[i],string[j]);
				strcpy(string[j],strtemp);
			}
		}
	}
	for(i=0;i<10;i++)
	{
		printf("%s\n",string[i]);
	}
	return 0;
}
