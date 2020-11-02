/*----------------------------------------
**	4.字符串原地压缩，例如：”eeeeeaaaff” 
**	压缩为 “e5a3f2”.
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

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

void main()
{
	char a[20];
	printf("input string:\n");
	scanf("%s",a);
	check(a);
}
