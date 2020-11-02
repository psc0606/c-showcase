/*----------------------------------------
**	2.编写一个函数，完成两个字符串的连接，并
**	输出新字符串。	
**----------------------------------------
*/

#include <stdio.h>
void cat(char *p,char *q)
{
	int i = 0, j;
	while ( *(p + i) != '\n')	//fgets以\n为结束标志
	{
		i++;
	}
	for (j = 0; *(q + j) != '\n'; i++,j++)
	{
		*(p + i) = *(q + j);
	}
	*(p + i) = '\0';
}

void main()
{
	char a[30],b[10];
	printf("input string a = ");
	fgets(a,30,stdin);		//fgets以\n为结束标志, 非\0
	printf("input string a = ");
	fgets(b,10,stdin);
	cat(a,b);
	printf("%s\n",a);
}

