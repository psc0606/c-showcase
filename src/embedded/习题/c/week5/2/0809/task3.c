#include<stdio.h>
//#define _STACK_H
#include<stdlib.h>
//#include"stack.h"

typedef int ElemType;

typedef struct node
{
	ElemType x;
	ElemType y;
	struct node* next;
}stack;

void initstack(stack** top)
{
	*top=(stack*)malloc(sizeof(stack));
	(*top)->next=NULL;
}

int is_empty(stack** top)
{
	return (*top)->next==NULL;
}

int push(stack** top,ElemType x,ElemType y)
{
	stack* s;
	s=(stack*)malloc(sizeof(stack));
	if(s==NULL) return 0;
	s->x=x;
	s->y=y;
	s->next=(*top)->next;
	(*top)->next=s;
	return 1;
}

int pop(stack** top,ElemType* x,ElemType* y)
{
	stack* s;
	if(is_empty(top))
	{
		printf("\nstack is free!");
		return 0;
	}
	s=(*top)->next;
	*x=s->x;
	*y=s->y;
	(*top)->next=s->next;
	free(s);
	return 1;
}

int getTop(stack** top,ElemType* x,ElemType* y)
{
	if(is_empty(top))
	{
		printf("\nstack is free!");
		return 0;
	}
	*x=(*top)->next->x;
	*y=(*top)->next->y;
	return 1;
}
	
int x=4,y=5;
void derection(int d,int i,int j)
{
	x=i;
	y=j;
	switch(d)
	{
		case 1:x--;break;
		case 2:y++;break;
		case 3:x++;break;
		default:y--;
	}
}

int main()
{
	int a[5][6]=
	{0,	0,	0,	1,	1,	1,
	0,	1,	0,	1,	0,	1,
	1,	0,	0,	1,	0,	1,
	1,	0,	1,	0,	0,	0,
	0,  0,	0,	0,	1,	0};
	int i=4,j=5,d=1,m,n,flag=0;
	stack* s=(stack*)malloc(sizeof(stack));
	while(!(i==0 && j==0))
	{
		if(a[x][y]==0)
		{
			flag=0;
			i=x;
			j=y;
			push(&s,i,j);
			a[i][j]=2;
			d=1;
			derection(d,i,j);
			if(x<0 || y<0 || x>4 || y>5)
			{
				d++;
				if(d>4)
					d=1;
				derection(d,i,j);
			}
		}
		else
		{
			flag++;
			if(flag==4)
			{
				a[i][j]=2;
				pop(&s,&i,&j);
				printf("i=%d,j=%d\n",i,j);
				printf("%d\n",a[i][j-1]);
				flag=0;
			}
			d++;
			getTop(&s,&i,&j);
			derection(d,i,j);
			if(d>4)
				d=1;
			if(x<0 || y<0 || x>4 || y>5)
			{
				d++;
				if(d>4)
					d=1;
				derection(d,i,j);
			}
		}
	}
	for(i=0;i<5;i++)
	{
		for(j=0;j<6;j++)
		{
			printf("%4d",a[i][j]);
		}
		printf("\n");
	}
	while(!is_empty(&s))
	{
		pop(&s,&m,&n);
		printf("坐标:(%d,%d)\n",m,n);
	}
}
