#ifndef _Stack_H
#define _Stack_H

#define MaxSize 3

typedef int elemtype;

typedef struct
{
	elemtype stack[MaxSize];
	int top;
}STACK;

void initStack(STACK *s);		//初始化
{
	s->top = -1;
}

int getTop(STACK *s,elemtype *x);
{
	*x = s->stack[s->top];
}

int empty(STACK *s);
{
	return s->top == -1;
}
int is_full(STACK *s);
{
	return s->top == MaxSize - 1;
}

int push(STACK *s,elemtype x);	//压栈
{
	if (is_full(s))
	{
		printf("is full\n");
		return -1;
	}
	s->top++;
	s->stack[s->top] = x;		//注意！
	return 0;
}

int pop(STACK *s,elemtype *x);
{
	if (empty(s))
	{
		printf("is empty");
		return -1;
	}
	*x = s->stack[s->top];
	s->top--;
	return 0;
}

#endif
