/*----------------------------------------
**	输入十个数，求他们的最小公倍数和最大公约数
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	long i;
	int  j, k, a[10];
	printf("input 10 numbers:\n");
	for (i = 0; i < 10; i++)
	{
		scanf("%d",&a[i]);
	}
	for (i = 1; i < 10; i++)
	{
		if (a[0] > a[i])
		{
			j = a[0];
			a[0] = a[i];
			a[i] = j;
		}
	}
	k = a[0];
	for (; k > 0; k--)
	{
		for (i = 1; i < 10; i++)
		{
			if (a[i] % k == 0)
			{
				continue;
			}
			else 
			{
				break;
			}
		}
		if (i == 10)
		{
			printf("Max = %d\n",k);
			break;
		}
	}
	for (i = 1;; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (i % a[j] == 0)
			{
				continue;
			}
			else 
			{
				break;
			}
		}
		if (j == 10)
		{
			printf("Min = %d\n",i);
			break;
		}
	}
}
