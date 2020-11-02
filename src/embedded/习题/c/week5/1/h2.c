/*----------------------------------------
**	2. 用两个栈模拟实现队列。
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

int getTop(stack **s)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	return (*s)->next->data;
}

int push(stack **s,elemtype data)
{
	stack *p = (stack *)malloc(sizeof(stack));
	p->next = (*s)->next;
	(*s)->next = p;
	p->data = data;
	return 0;
}

int pop(stack **s)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	stack *p = (*s)->next;
	(*s)->next = p->next;
	free(p);
	return 0;
}

void main()
{
	int i, n;
	stack *s,*p;
	initstack(&s);
	initstack(&p);
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		push(&s,n);
	}
	while(!empty(&s))
	{
		push(&p,getTop(&s));
		pop(&s);
	}
	while(!empty(&p))
	{
		printf("%d",getTop(&p));
		pop(&p);
	}
}
