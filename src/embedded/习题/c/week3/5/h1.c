/*----------------------------------------
**	1. 定义一个包括年、月、日的结构体变量，
**	当输入年、月、日数据后，计算该日是这一年
**	中的第几天。
**----------------------------------------
*/

#include <stdio.h>

typedef struct
{
	int year;
	int month;
	int day;
}date;

void main()
{
	date *p,b;
	p = &b;			//必须指定地址！
	int i, day = 0,m[12]={31,28,31,30,31,30,
							31,31,30,31,30,31};
	printf("input year month day :\n");
	scanf("%d%d%d",&p->year,&p->month,&p->day);
	
	for (i = 0; i < p->month-1; i++)
	{
		day += m[i];
	}
	day = day + p->day;
	if ((p->year%4==0 && p->year%100==0)||
		(p->year%400==0))
	{
		if (p->month > 2)
		{
			day++;
		}
	}
	printf("The day is the %d day in the year!",day);
}


/*
void main()
{
	date p;
	int i, day = 0,m[12]={31,28,31,30,31,30,
							31,31,30,31,30,31};
	printf("input year month day :\n");
	scanf("%d%d%d",&p.year,&p.month,&p.day);
	
	for (i = 0; i < p.month-1; i++)
	{
		day += m[i];
	}
	day = day + p.day;
	if ((p.year%4==0 && p.year%100==0)||
		(p.year%400==0))
	{
		if (p.month > 2)
		{
			day++;
		}
	}
	printf("The day is the %d day in the year!",day);
}
*/
