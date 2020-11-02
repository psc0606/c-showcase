#include <stdio.h>
#include <string.h>
void check(char *a, char *b,int(*cmp)
			(const char *,const char *))
{
	printf("== |!=\n");
	if (!(*cmp)(a,b))
	{
		printf("==\n");
	}
	else
	{
		printf("!=\n");
	}
}
void main()
{
	char s1[80],s2[80];
	int (*p)(const char *,const char *);
	p = strcmp;
	printf("input string:\n");
	fgets(s1,80,stdin);
	fgets(s2,80,stdin);
	check(s1,s2,p);
}
