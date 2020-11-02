/*----------------------------------------
**	2. 实现链式堆栈的基本操作
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>

typedef int elemtype;

typedef struct node
{
	elemtype data;
	struct node *next;
}stack;

void initstack(stack **s)
{
	*s = (stack *)malloc(sizeof(stack));
	(*s)->next = NULL;
}

int empty(stack **s)
{
	return (*s)->next == NULL;
}

int getTop(stack **s,elemtype data)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return 0;
	}
	data = (*s)->next->data;
	return data;
}

int push(stack **s,elemtype data)
{
	stack *p = (stack *)malloc(sizeof(stack));
	if (p == NULL)
	{
		printf("error\n");
		return -1;
	}
	p->data = data;
	p->next = (*s)->next;
	(*s)->next = p;
	return 0;
}

int pop(stack **s,elemtype data)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	stack *p = (*s)->next;
	data = (*s)->data;
	(*s)->next = p->next;
	free(p);
	return 1;
}

void main()
{
	int i, n, m;
	stack *s;
	initstack(&s);
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		push(&s,n);
	}
	while(!empty(&s))
	{
		printf("%d",getTop(&s,m));
		pop(&s,m);
	}
}
