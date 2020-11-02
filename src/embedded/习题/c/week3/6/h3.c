/*----------------------------------------
**	3.加密
**	TRAILBLAZERS  去掉重复字母后  TRAILBZES
**	ABCDEFGHIJKLMNOPQRSTUVWXYZ
**	TRAILBZESCDFGHJKMNOPQUVWXY
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void check(char *p)
{
	int i, j;
	for (i = 0; *(p+i) != '\0'; i++)
	{
		for (j = i+1; *(p+j) != '\0'; j++)
		{
			if (*(p+i) == *(p+j))
			{
				strcpy(p+j,p+j+1);
				j--;
			}
		}
	}
	printf("\n%s\n",p);
}

char *change(char *p,char *q)
{
	char b[100], *a;
	a = b;
	strcpy(b,p);
	strcpy(b+strlen(p),q);
	check(b);
	return a;
}

void main()
{
	int i, j;
	char a[20]={"TRAILBLAZERS"}, b[27],c[20],n[100],*d,m[100];
	d = n;
	printf("input the word:\n");
//	scanf("%s",a);
	for (i = 0; i < 26; i++)
	{
		b[i] = 'A'+i;
		printf("%c",'A'+i);
	}
	b[i] = '\0';
	printf("\ninput the string:\n");
	gets(c);
	check(a);
	d = change(a,b);
	for (i = 0; c[i] != '\0';i++)
	{
		for (j = 0; b[j] != '\0' ; j++)
		{
			if (b[j] == c[i])
			{
				m[i] = *(d+j);
				break;
			}
			if (c[i] == ' ')
			{
				m[i] = ' ';
				break;
			}
		}
	}
	printf("%s\n",m);
}
