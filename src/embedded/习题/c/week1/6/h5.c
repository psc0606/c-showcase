/*----------------------------------------
**	5. 在屏幕上显示0度到360度的cos曲线
**	与直线f(x)=4*(y-1)+3的迭加图形
**	其中cos（x）图形用“*”表示，f(x)使用“+”表示
**----------------------------------------
*/

#include <stdio.h>
#include <math.h>
void main()
{
	int i, j;
	double x1, x2, y;
	for (y = 10; y >= -10; y--)
	{
		x1 = acos(y/10) * 10;
		x2 = (y + 1) / 4;
		if (x1 < x2)
		{
			for (i = 0; i <= x1; i++)
			{
				printf(" ");
			}
			printf("*");
			for (j = 0; j <= x2-x1; j++)
			{
				printf(" ");
			}
			printf("+");
			for (i = 0;i <= 62 - x1 - x2; i++)
			{
				printf(" ");
			}
			printf("*");
			printf("\n");
		}
		else
		{
			for (i = 0; i <= x1; i++)
			{
				printf(" ");
			}
			printf("*");
			for (i = 0;i <= 62 - 2*x1; i++)
			{
				printf(" ");
			}
			printf("*\r");
			for (j = 0; j <= x2; j++)
			{
				printf(" ");
			}
			printf("+");
			printf("\n");
		}
	}
}
