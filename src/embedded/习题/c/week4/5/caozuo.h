#ifndef _stack_caozuo
#define _stack_caozuo

typedef int elemtype;

typedef struct node;
{
	elemtype data;
	struct node *next;
}stack;

void initstackc(stack **s)
{
	*s = (stack *)malloc(sizeof(stack));
	(*s)->next = NULL;
}

int emptyc(stack **s)
{
	return (*s)->next == NULL;
}

int getTopc(stack **s,elempty data)
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

int pushc(stack **s,elempty data)
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

int popc(stack **s,elemtype data)
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
