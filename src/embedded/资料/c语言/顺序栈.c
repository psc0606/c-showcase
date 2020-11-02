/*----------------------------------------
**	1. 用动态分配数组实现栈的基本操作。
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>
#define MaxSize 5

typedef int elemtype;

typedef struct node
{
	elemtype data[MaxSize];
	int top;
}stack;

void initstack(stack *s)
{
	s->top = -1;
}

int empty(stack *s)
{
	return s->top == -1;
}

int is_full(stack *s)
{
	return s->top == MaxSize-1;
}

int push(stack *s,elemtype data)
{
	if (is_full(s))
	{
		printf("stack is full\n");
		return -1;
	}
	s->top++;
	s->data[s->top] = data;
	return 0;
}

int pop(stack *s,elemtype *data)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	*data = s->data[s->top--];
	return 0;
}

int getTop(stack *s,elemtype *data)
{
	if(empty(s))
	{
		printf("stack is empty!\n");
		return 0;
	}
	*data = s->data[s->top];
	return *data;
}

void main()
{
	int i, n, m;
	stack *s = (stack *)malloc(sizeof(stack));
	initstack(s);
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		push(s,n);
	}
	while(!empty(s))
	{
		printf("%d",getTop(s,&m));
		pop(s,&m);
	}
	free(s);
}
