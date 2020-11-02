#ifndef _stack_yunsuan
#define _stack_yunsuan

typedef char elemtype;

typedef struct node;
{
	elemtype data;
	struct node *next;
}stack;

void initstacky(stack **s)
{
	*s = (stack *)malloc(sizeof(stack));
	(*s)->next = NULL;
}

int emptyy(stack **s)
{
	return (*s)->next == NULL;
}

int getTopy(stack **s,elempty data)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return 0;
	}
	stack *p = (*s)->next;
	data = p->data;
	(*s)->next = p->next;
	free(p);
	return 1;
}

int pushy(stack **s,elempty data)
{
	stack *p = (stack *)malloc(stack));
	if (p == NULL)
	{
		printf("error\n");
		return -1;
	}
	p->data = data;
	p = (*s)->next;
	(*s)->next = p;
	return 0;
}

int popy(stack **s,elemtype data)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	stack *p = (*s)->next;
	data = p->data;
	(*s)->next = p->next;
	free(p);
}

#endif
