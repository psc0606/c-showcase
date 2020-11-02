#ifndef _stack_link_H
#define _stack_link_H

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

int getTop(stack **s,elemtype *data)
{
	if (empty(s))
	{
		printf("\nstack is free!\n");
		return 1;
	}
	*data = (*s)->next->data;
	printf("%d",*data);
	return 0;

}

int push(stack **s,elemtype data)
{
	stack *p = (stack *)malloc(sizeof(stack));
	if (p == NULL)
	{
		return 1;
	}
	p->data = data;
	p->next = (*s)->next;
	(*s)->next = p;
	return 0;
}

int pop(stack **s,elemtype *data)
{
	if (empty(s))
	{
		printf("\nstack is free!\n");
		return -1;
	}
	stack *p = (*s)->next;
	*data = p->data;
	(*s)->next = p->next;
	free(p);
	return 0;
}

#endif
