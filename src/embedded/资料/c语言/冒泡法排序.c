/*--------------------------------
	冒泡法排序
----------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j, n;
	printf("input n = ");
	scanf("%d",&n);
	char k, a[n];
	printf("input a[i] = ");
	scanf("%s",a);
	for (i = 0; i < n-1; i++)
	{
		for (j = n-1; j > i; j--)
		{
			if (a[j] < a[j-1])
			{
				k = a[j];
				a[j] = a[j-1];
				a[j-1] = k;
			}
		}
	}
	printf("%s",a);
}
