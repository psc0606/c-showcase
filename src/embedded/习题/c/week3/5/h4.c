/*----------------------------------------
**	4. 定义一种类型stu可以放学生的姓名、学号、
**	英语成绩其中英语成绩有的老师按照“优”、“良”、
**	“中”、“及格”、“不及格”五个等级给的，有的老
**	师按照百分制给的成绩，要求该类型可以如实反映
**	学生的情况。
**----------------------------------------
*/

#include <stdio.h>
#define x 3

struct
{
	char name[10];
	char num[10];
	int score;
}stu[x];

char sort(int a)
{
	switch (a/10)
	{
		case 9:
			return 'A';
			break;
		case 8:
			return 'B';
			break;
		case 7:
			return 'C';
			break;
		case 6:
			return 'D';
			break;
		default:
			return 'E';
			break;
	}
}

void main()
{
	int i;
	for (i = 0; i < x; i++)
	{
		scanf("%s%s%d",stu[i].name,stu[i].num,&stu[i].score);
	}
	
	for (i = 0; i < x; i++)
	{
		printf("%s\t%s\t%d\t%c\n",stu[i].name,stu[i].num,stu[i].score,sort(stu[i].score));
	}
}
