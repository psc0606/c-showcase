/*----------------------------------------
**	2. 从键盘输入n个单词，然后进行从大到小的
**	顺序排序，输出排序后的结果（要求用选择排序法）
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void main()
{
	int i, j, k, n;
	printf("input n = ");
	scanf("%d",&n);
	char a[n][10], b[10];
	for (i = 0; i < n; i++)
	{
		scanf("%s",a[i]);
	}
	
	for (i = 0; i < n - 1; i++)
	{
		k = i;
		for (j = i + 1; j < n; j++)
		{
			if (strcmp(a[k],a[j]) < 0)
			{
				k = j;
			}
		}
		if (k != i)
		{
			strcpy(b,a[i]);
			strcpy(a[i],a[k]);
			strcpy(a[k],b);
		}
	}
	
	printf("The word is\n");
	for (i = 0; i < n; i++)
	{
		printf("%s\n",a[i]);
	}
}
