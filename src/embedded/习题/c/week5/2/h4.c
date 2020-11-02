/*----------------------------------------
**	4. 使用动态分配和指针数组的方法对若干个字
**	符串排序。（排序方法使用插入排序法）。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define N 2

void sort(char **p)
{
	int i, j;
	for (i = 1; i < N; i++)
	{
		j = i-1;
		char a[10];
		strcpy(a,p[i]);
		while (j >= 0 && strcmp(a,p[j]) < 0)
		{
			strcpy(p[i--],p[j--]);
		}
		strcpy(p[i],a);
	}
}

void main()
{
	int i;
	char **p;
	p = (char **)malloc(N*sizeof(char *));
	for (i = 0; i < N; i++)
	{
		p[i] = (char *)malloc(10*sizeof(char));
		scanf("%s",p[i]);
	}
	sort(p);
	printf("-------------sort-------------\n");
	for (i = 0; i < N; i++)
	{
		printf("%s\n",p[i]);
	}
	for (i = 0; i < N; i++)
	{
		free(p[i]);
	}
	free(p);
}
