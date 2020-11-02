#include<stdio.h>
#include<stdlib.h>

void initstack(stack** top)
{
	*top=(stack*)malloc(sizeof(stack));
	(*top)->next=NULL;
}

int is_empty(stack** top)
{
	return (*top)->next==NULL;
}

int push(stack** top,ElemType x)
{
	stack* s;
	s=(stack*)malloc(sizeof(stack));
	if(s==NULL) return 0;
	s->data=x;
	s->next=(*top)->next;
	(*top)->next=s;
	return 1;
}

int pop(stack** top,ElemType* x)
{
	stack* s;
	if(is_empty(top))
	{
		printf("\nstack is free!");
		return 0;
	}
	s=(*top)->next;
	(*top)->next=s->next;
	free(s);
	return 1;
}

int getTop(stack** top,ElemType* x)
{
	if(is_empty(top))
	{
		printf("\nstack is free!");
		return 0;
	}
	*x=(*top)->next->data;
	return 1;
}

