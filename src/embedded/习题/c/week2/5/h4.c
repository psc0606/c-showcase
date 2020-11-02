/*----------------------------------------
**	4.有一个已经排好序的数组。现输入一个数，
**	要求按原来的规律将它插入数组中。
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j, k, n, *p, *q;
	int a[9]= {1,4,8,12,16,22,26,30};
	int b[9]= {30,26,22,16,12,8,4,1};
	p = a;
	q = b;
	for (i = 0; i < 8; i++)
	{
		printf("%4d",*(p+i));
	}
	printf("\n");
	printf("input n = ");
	scanf("%d",&n);
	for (i = 0; i < 9; i++)
	{
		if (*(p + i) >= n)
		{
			for (j = 8; j > i; j--)
			{
				*(p + j) = *(p + j - 1);
			}
			*(p + i) = n;
			break;
		}
		else
		{
			*(p + 8) = n;
		}
	}
	
	for (i = 0; i < 9; i++)
	{
		if (*(q + i) <= n)
		{
			for (j = 8; j > i; j--)
			{
				*(q + j) = *(q + j - 1);
			}
			*(q + i) = n;
			break;
		}
		else
		{
			*(q + 8) = n;
		}
	}
	
	
	for (i = 0; i < 9; i++)
	{
		printf("%4d",*(p+i));
	}
	printf("\n");
	for (i = 0; i < 9; i++)
	{
		printf("%4d",*(q+i));
	}
	printf("\n");
}
