#ifndef _STACK_H
#define _STACK_H

typedef int ElemType;

typedef struct node
{
	ElemType data;
	struct node* next;
}stack;

void initstack(stack**);

int is_empty(stack**);

int push(stack**,ElemType);

int pop(stack**,ElemType*);

int getTop(stack**,ElemType*);

#endif
