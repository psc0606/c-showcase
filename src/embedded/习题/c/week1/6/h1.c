/*----------------------------------------
**	1. 输入一个不多于5位的正整数，要求：
**	1、求它是几位数，2、按逆序输出各位数字。
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, j = 0, n;
	printf("input the number:\n");
	scanf("%d",&n);
	for (i = 1; i <= 5; i++)
	{
		j++;
		if (n / 10 != 0)
		{
			printf("%d",n % 10);
			n = n / 10;
		}
		else 
		{
			printf("%d",n);
			break;
		}
	}
	printf("\nnum = %d\n",j);
}
