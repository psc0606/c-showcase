/*----------------------------------------
**	1. 一个学生的信息是：姓名，学号，性别，
**	年龄等信息，用一个链表，把这些学生信息
**	连在一起，给出一个age，在链表中删除学
**	生年龄等于age的学生信息。
**	（要求能够实现循环删除）。
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
typedef struct student
{
	char name[10];
	char num[10];
	char sex;
	int age;
	struct student *next;
}stu;

stu *creat_stu(char *name,char *num,char sex,int age)
{
	stu *new = (stu *)malloc(sizeof(stu));
	if (new == NULL)
	{
		printf("error!\n");
		return new;
	}
	strcpy(new->name,name);
	strcpy(new->num,num);
	new->sex = sex;
	new->age = age;
	new->next = NULL;
	return new;
}

stu *insert(stu *s,stu *new)
{
	if (new == NULL)
	{
		printf("error!\n");
		return s;
	}
	if (s == NULL)
	{
		s = new;
	}
	else
	{
		stu *p = s;
		for (; p->next != NULL; p = p->next)
		{}
		p->next = new;
	}
	return s;
}

stu *delete(stu *s,int age)
{
	stu *p, *q;
	int n = 1;
	while (1)
	{
		for (p = s; p != NULL; p = p->next)
		{
			if (p->age == age)
			{
				n = 0;
				break;
			}
			q = p;
		}
		if (p == NULL)
		{
			if (n)
			{
				printf("no find!\n");
			}
			return s;
		}
		else if (p == s)
		{
			s = p->next;
		}
		else
		{
			q->next = p->next;
		}
		free(p);
	}
	return s;
}

void display(stu *s)
{
	stu *p = s;
	printf("姓名\t学号\t性别\t年龄:\n");
	for (; p != NULL; p = p->next)
	{
		printf("%s\t%s\t%c\t%d\n",p->name,p->num,p->sex,p->age);
	}
}

void destroy(stu *s)
{
	stu *p = s;
	for (;p != NULL; p = s)
	{
		s = p->next;
		free(p);
	}
}

void main()
{
	int i, age;
	char name[10],num[10],sex;
	char ch;
	stu *s = NULL;
	for (i = 0; i < 3; i++)
	{
		printf("姓名，学号，性别，年龄:\n");
		scanf("%s%s",name,num);
		getchar();
		scanf("%c",&sex);
		scanf("%d",&age);
		s = insert(s,creat_stu(name,num,sex,age));
	}
	while (s)
	{
		display(s);
		printf("input del:age:	按q退出!\n");
		scanf("%d",&age);
		if (ch = getchar() == 'q')
		{
			break;
		}
		s = delete(s,age);
	}
	destroy(s);
}
