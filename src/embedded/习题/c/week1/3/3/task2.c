#include<stdio.h>

int leap(int year)
{
	int leap;
	leap = (year%4==0 && (year%100!=0 || year%400==0));
	return leap;
}

int sum_day(int month,int day)
{
	int day_tab[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	int i;
	for(i=1;i<month;i++)
		day+=day_tab[i];
	return day;
}

int fun(int year,int month,int day)
{
	int days;
	days=sum_day(month,day);
	if(leap(year) && month>=3)
		days=days+1;
	return days;
}

int main()
{
	int year1,month1,day1;
	int year2,month2,day2;
	int days1,days2;
	int num1=0;
	int num2=0;
	int i;
	long int n=0;
	printf("please input two date :");
	scanf("%d,%d,%d",&year1,&month1,&day1);
	scanf("%d,%d,%d",&year2,&month2,&day2);
	days1=fun(year1,month1,day1);
	days2=fun(year2,month2,day2);
	for(i=year1;i<year2;i++)
	{
		if(leap(i)==1)
			num1++;
		else
			num2++;
	}
	n=366*num1+365*num2+days2-days1;
	printf("n=%d\n",n);
	if((n+1)%5>=1 || (n+1)%5<=3)
		printf("打鱼\n");
	else
		printf("晒网\n");
}
