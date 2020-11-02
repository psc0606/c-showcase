/*----------------------------------------
**	2.编写一个函数,打印一个学生的成绩组数，
**	该组中有5个学生的数据记录，每个记录包括
**	num、name、socre[3],并按姓名排序。
**	（要求用结构体实现）
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
#define x 5

struct student
{
	char num[10];
	char name[10];
	int score[3];
}stu[x+1];

void display(struct student stu[])
{
	int i, j, k, n;
	for (i = 0; i < x-1; i++)
	{
		k = i;
		for (j = i+1; j < x; j++)
		{
			if (strcmp(stu[k].name,stu[j].name) > 0)
			{
				k = j;
			}
			if (k != i)
			{
				strcpy(stu[x].num,stu[k].num);
				strcpy(stu[k].num,stu[i].num);
				strcpy(stu[i].num,stu[x].num);
				strcpy(stu[x].name,stu[k].name);
				strcpy(stu[k].name,stu[i].name);
				strcpy(stu[i].name,stu[x].name);
				for (n = 0; n < 3; n++)
				{
					stu[x].score[n] = stu[k].score[n];
					stu[k].score[n] = stu[i].score[n];
					stu[i].score[n] = stu[x].score[n];
				}
			}
		}
	}
	for (i = 0; i < x; i++)
	{
		printf("%s\t%s\t%d\t%d\t%d\n",stu[i].num,stu[i].name,
		stu[i].score[0],stu[i].score[1],stu[i].score[2]);
	}
}

void main()
{
	int i;
	for (i = 0; i < x; i++)
	{
		printf("输入%d个学生的学号:\n",i+1);
		scanf("%s",stu[i].num);
		printf("输入%d个学生的姓名:\n",i+1);
		scanf("%s",stu[i].name);
		printf("输入%d个学生三门课的成绩:\n",i+1);
		scanf("%d%d%d",&stu[i].score[0],
				&stu[i].score[1],&stu[i].score[2]);
	}
	display(stu);
}
