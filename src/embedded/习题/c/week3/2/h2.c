/*----------------------------------------
**	2. 从键盘输入10个数，利用条件编译将其
**	中最大值与最小值显示出来。
**----------------------------------------
*/

#include <stdio.h>

void main()
{
	int i, a[10], min, max;
	printf("input 10 numbers:\n");
	for (i = 0; i < 10; i++)
	{
		scanf("%d",a+i);
	}
	min = max = a[0];
	for (i = 1; i < 10; i++)
	{
		#if 1
		{
			if (min > a[i])
			{
				min = a[i];
			}
			if (max < a[i])
			{
				max = a[i];
			}
		}
		#else 
		{
		}
		#endif
	}
	printf("Max = %d,Min = %d\n",max,min);
}

