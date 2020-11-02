/*----------------------------------------
**	2. 实行行程压缩，将重复的数据用重复次数来
**	表达，例如：aaabbbccc->a3b3c3,
**	abcd->abcd,aabcdd->a2bcd2.(用指针实现)。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
/*
void check(char *a)
{
	int i, j, k = 0;
	for (i = 0; *(a+i) != '\0'; i++)
	{
		printf("%c",*(a+i));
		if (*(a+i) == *(a+i+1))
		{
			for(j = i; *(a+j) == *(a+j+1); j++)
			{}
			printf("%d",(j+1-i));
			i = j;
		}
	}
}
*/
void check(char *a)
{
	int i, j, k = 0;
	char b[20];
	for (i = 0; *(a+i) != '\0'; i++)
	{
		b[k++] = *(a+i);
		if (*(a+i) == *(a+i+1))
		{
			for(j = i; *(a+j) == *(a+j+1); j++)
			{}
			b[k++] = j+1-i + '0';
			i = j;
		}
	}
	b[k] = '\0';
	printf("%s",b);
}


void main()
{
	char a[20];
	printf("input string:\n");
	scanf("%s",a);
	check(a);
}
