/*----------------------------------------
**	3. 输入一段英文，对其中字符串进行替换，
**	例如hgabcabckkkk，把abc替换成efg，字
**	符串成为hgefgefgkkkk。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
void swap(char a[],char b[],char c[])
{
	int i, j, k, l, m, n;	
	l = strlen(a);
	m = strlen(b);
	n = strlen(c);
	char p[m+1];
	for (i = 0; i <= l-m; i++)
	{
		for (j = 0; j < m; j++)
		{
			p[j] = a[i+j];
		}
		p[j] = '\0'; //很重要！！！！！！！！！
		if (strcmp(p,b) == 0)
		{
			for (k = 0; k < n; k++)
			{
				*(a+i+k) = *(c+k);
			}
		}
	}
}

void main()
{
	char a[20], b[20], c[20];
	printf("input string a:\n");
	scanf("%s",a);
	printf("input string b:\n");
	scanf("%s",b);
	printf("input string c:\n");
	scanf("%s",c);
	swap(a,b,c);
	printf("%s",a);
}
