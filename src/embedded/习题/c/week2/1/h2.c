/*----------------------------------------
**	编写函数int palindrome(char string[]) 
**	如果参数字符串是个回文就返回1，否则返回0
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
int palindrom(char string[])
{
	int i, n;
	n = strlen(string);
	for (i = 0; i < n/2; i++)
	{
		if (string[i] == string[n-i-1])
		{
			continue;
		}
		else
		{
			break;
		}
	}
	if (i == n/2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void main()
{
	char a[10];
	scanf("%s",a);
	printf("%d",palindrom(a));
}
