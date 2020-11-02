/*----------------------------------------
**	2. 编写函数reverse_string(char *str);
**	函数把参数字符串中的字符反向排序，请使用指
**	针而不是数组下标，不要使用任何C函数库中用
**	于操作字符串的函数。不需要声明一个局部数
**	组来临时存储参数字符串。
**----------------------------------------
*/

#include <stdio.h>

void reverse_string(char *str)
{
	int i = 0;
	while(*(str+i) != '\0')
	{
		i++;
	}
	for (; i >= 0; i--)
	{
		printf("%c",*(str+i));
	}
}

void main()
{
	char a[20], *p;
	scanf("%s",a);
	p = a;
	reverse_string(a);
}
