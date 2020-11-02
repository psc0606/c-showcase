/*----------------------------------------
**	4.输入主字符串和子字符串，如果主字符串
**	不包含子字符串，则打印提示信息，如果主
**	字符串包含子字符串，删除所有的子字符串。
**	并打印删除后的字符串。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void check(char *a, char *b)
{
	char c[20];
	int i, j = 0, len = 0;
	while (*(b+len)!= '\n')
	{
		len++;
	}
	*(b+len) = '\0';
	for (i = 0; *(a+i) != '\0'; i++)
	{
		if (*b == *(a+i))
		{
			memcpy(c,(a+i),len*sizeof(char));
			c[len] = '\0';
			if (strcmp(c,b) == 0)
			{
				j = 1;
				strcpy((a+i),(a+i+len));
				i--;
			}
		}
	}
	if(j == 1)
	{
		printf("%s\n",a);
	}
	else
	{
		printf("error\n");
	}
	
}

void main()
{
	char a[20], b[20];
	printf("input string a:");
	fgets(a,20,stdin);
	printf("input string b:");
	fgets(b,20,stdin);
	check(a,b);
}
