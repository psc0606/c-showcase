/*----------------------------------------
**	输入十个字符串，对其进行排序
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
void main()
{
	int i, j;
	char str[10], a[10][10];
	for (i = 0; i < 10; i++)
	{
		scanf("%s",&a[i]);
	}
	for (j = 0; j < 9; j++)
	{
		for (i = 0; i < 9-j; i++)
		{
			if (strcmp(a[i],a[i+1])>0)
			{
				strcpy(str,a[i]);
				strcpy(a[i],a[i+1]);
				strcpy(a[i+1],str);
			}	
		}
	}
	printf("从小到大排序为\n");
	for (i = 0; i < 10; i++)
	{
		printf("%s\n",a[i]);
	}
}
