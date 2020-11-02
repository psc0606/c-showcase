/*----------------------------------------
**	1.递归实现回文判断。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void check(char *a,int len)
{
	int i =0;
	if (len == 1 || len == 0)
	{
		printf("是回文!\n");
		return;
	}
	else if (*(a+i) != *(a+len-1-i))
	{
		printf("不是回文!\n");
		return;
	}
	else
	{
		i++;
		len -= 2;
		check(a+i,len);
	}
}

void main()
{
	char a[10];
	printf("input the string:");
	scanf("%s",a);
	static int len;
	len = strlen(a);
	check(a,len);
}
