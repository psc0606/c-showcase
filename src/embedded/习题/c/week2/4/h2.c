/*----------------------------------------
**	2.编写一个函数
**	string_cut(char *str ,char c）,如果字
**	符串str中包含字符变量c，则输出其后的字符
**序列，否则输出The character isn't included.
**----------------------------------------
*/

#include <stdio.h>

void string_cut(char *str, char c)
{
	int i;
	for (i = 0; *(str+i) != '\0'; i++)
	{
		if (*(str+i) == c)
		{
			printf("%s\n",str+i+1);
			break;
		}
	}
	if (*(str+i) == '\0')
	{
		printf("The character isn't included\n");
	}
}
void main()
{
	char c, a[20];
	printf("input string:\n");
	scanf("%s",a);
	printf("input the word:\n");
	getchar();
	scanf("%c",&c);
	string_cut(a,c);
}
