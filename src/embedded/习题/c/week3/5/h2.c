/*----------------------------------------
**	2. 从键盘输入10个学生的数据，包括：学号、
**	姓名、三门课的成绩。要求输出三门课总平均
**	成绩，及最高分学生的数据（学号、姓名、三
**	门课成绩、平均分数）。
**----------------------------------------
*/

#include <stdio.h>
#define x 3

struct student
{
	int num;
	char name[10];
	int score1;
	int score2;
	int score3;
}stu[x];

void main()
{
	int i, j = 0, max;
	float score[x];
	for (i = 0; i < x; i++)
	{
		printf("输入%d个学生的学号:\n",i+1);
		scanf("%d",&stu[i].num);
		printf("输入%d个学生的姓名:\n",i+1);
		scanf("%s",&stu[i].name);
		printf("输入%d个学生三门课的成绩:\n",i+1);
		scanf("%d%d%d",&stu[i].score1,
				&stu[i].score2,&stu[i].score3);
		score[i] = stu[i].score1 + stu[i].score2
					 + stu[i].score3;
	}
	
	printf("学号:	姓名:	三门课的成绩:	平均分数:\n",i+1);
	for (i = 0; i < x; i++)
	{
		printf("%-5d%7s%6d%4d%4d%12.2f\n",stu[i].num,stu[i].name,
		stu[i].score1,stu[i].score2,stu[i].score3,score[i]/3);
	}
	
	max = score[0];
	for (i = 0; i < x; i++)
	{
		if (max < score[i])
		{
			max = score[i];
			j = i;
		}
	}
	
	for (i = 0; i < x; i++)
	{
		if (score[i] == score[j])
		{
			printf("最高分的学生:\n");
			printf("%-5d%7s%6d%4d%4d%12.2f\n",stu[i].num,stu[i].name,
			stu[i].score1,stu[i].score2,stu[i].score3,score[i]/3);
		}
	}
}
