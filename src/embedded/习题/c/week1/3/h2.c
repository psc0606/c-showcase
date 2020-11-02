/*----------------------------------------
**	2、 某人从1990年1月1日起开始
**	“三天打鱼两天晒网”，问这个人在以后的
**	 某一天是在打鱼还是在晒网，某一天由键盘输入
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int year, y, month, day, d, i, n=0;
	printf("请输入年.月.日:\n");
	scanf("%d.%d.%d",&year,&month,&day);
	int m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	y = year - 1988;
	for (i = 0; i < month-1; i++)
	{
		n += m[i];
	}
	if (y % 4 != 0)
	{
		d = y /4 * 366 + (year - 1990 - y / 4) *365 + n + day;
		printf("%d天\n",d);
	}
	else
	{
		d = (y / 4 - 1) * 366 +(year - 1990 - y / 4 +1) * 365 + n + day;
		if (month > 2)
		{
			d = d + 1;
		}
		printf("%d天\n",d);
	}
	
	
	switch (d % 5)
	{
		case 1:
			printf("打鱼\n");
			break;
		case 2:
			printf("打鱼\n");
			break;
		case 3:
			printf("打鱼\n");
			break;
		case 4:
			printf("晒网\n");
			break;
		case 0:
			printf("晒网\n");
			break;
	}
}
