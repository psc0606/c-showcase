/*----------------------------------------
**	1. 从键盘随意输入n个字符串，要求对输入
**	的字符串进行从小到大排序，然后输出结果
**	（要求用冒泡排序法）。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void main()
{
	int i, j, n;
	printf("input n = ");
	scanf("%d",&n);
	char a[n][10], b[n];
	for (i = 0; i < n; i++)
	{
		scanf("%s",a[i]);
	}
	
	for (i = 0; i < n-1; i++)
	{
		for (j = 0; j < n-i-1; j++)
		{
			if (strcmp(a[j],a[j+1]) > 0)
			{
				strcpy(b,a[j]);
				strcpy(a[j],a[j+1]);
				strcpy(a[j+1],b);
			}
		}
	}
	
	printf("The string is \n");
	for (i = 0; i < n; i++)
	{
		printf("%s\n",a[i]);
	}
}
