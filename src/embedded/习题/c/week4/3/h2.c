/*----------------------------------------
**	2. 已知两个双链表，每个链表的节点包括指向
**	前面一个节点的指针，学号、成绩和指向下一个
**	节点的指针，要求写个函数，将两个链表合并，
**	并分别写出对学号和成绩的排序；
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct student
{
	struct student *fwd;
	char num[10];
	int score;
	struct student *bwd;
}stu;

stu *new_stu(char num[], int score)
{
	stu *p = (stu *)malloc(sizeof(stu));
	p->fwd = NULL;
	strcpy(p->num,num);
	p->score = score;
	p->bwd = NULL;
	return p;
}

stu *insert_stu(stu *head,stu *new_stu)
{
	if (new_stu == NULL)
	{
		printf("empty\n");
		return head;
	}
	if (head != NULL)
	{
		new_stu->bwd = head;
		new_stu->bwd->fwd = new_stu;
	}
	head = new_stu;
	return head;
}

stu *link_stu(stu *a,stu *b)
{
	stu *p = a;
	for (; p->bwd != NULL; p = p->bwd)
	{}
	p->bwd = b;
	b->fwd = p;
	return a;
}

stu *sort_stu_num(stu *head)
{
	stu *p, *q, *m;
	int score;
	char temp[10];
	for (p = head; p != NULL; p = p->bwd)
	{
		q = p;
		for (m = p->bwd; m != NULL; m = m->bwd)
		{
			if (strcmp(q->num,m->num) > 0)
			{
				q = m;
			}
		}
		if (p != q)
		{
			strcpy(temp,p->num);
			strcpy(p->num,q->num);
			strcpy(q->num,temp);
			score = p->score;
			p->score = q->score;
			q->score = score;
		}
	}
	return head;
}

stu *sort_stu_score(stu *head)
{
	stu *p, *q, *m;
	int score;
	char temp[10];
	for (p = head; p != NULL; p = p->bwd)
	{
		q = p;
		for (m = p->bwd; m != NULL; m = m->bwd)
		{
			if (q->score < m->score)
			{
				q = m;
			}
		}
		if (p != q)
		{
			strcpy(temp,p->num);
			strcpy(p->num,q->num);
			strcpy(q->num,temp);
			score = p->score;
			p->score = q->score;
			q->score = score;
		}
	}
	return head;
}

void display(stu *head)
{
	stu *p = head;
	for (; p != NULL; p = p->bwd)
	{
		printf("%s\t%d\n",p->num,p->score);
	}
	printf("\n");
}

void destroy(stu *head)
{
	stu *p = head;
	for (; p != NULL; p = head)
	{
		head = p->bwd;
		free(p);
	}
}

void main()
{
	int i, score;
	char num[10];
	stu *head, *a = NULL, *b = NULL;
	printf("input a:\n");
	for (i = 0; i < 3; i++)
	{
		printf("input num:");
		scanf("%s",num);
		printf("input score:");
		scanf("%d",&score);
		a = insert_stu(a,new_stu(num,score));
	}
	printf("input b:\n");
	for (i = 0; i < 3; i++)
	{
		printf("input num:");
		scanf("%s",num);
		printf("input score:");
		scanf("%d",&score);
		b = insert_stu(b,new_stu(num,score));
	}
	display(a);
	display(b);
	a = link_stu(a,b);
	display(a);
	a = sort_stu_num(a);
	printf("按学号排序:\n");
	display(a);
	a = sort_stu_score(a);
	printf("按成绩排序:\n");
	display(a);
	destroy(a);
}
