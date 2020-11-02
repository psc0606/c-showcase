/*----------------------------------------
**	3．用栈和队列实现回文判断。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef char elemtype;

typedef struct qnode
{
	elemtype data;
	struct qnode *next;
}Qnode;

typedef struct 
{
	Qnode *front, *rear;
}queue;

void initqueue(queue *q)
{
	q->front = q->rear = (Qnode *)malloc(sizeof(Qnode));
	q->front->next = NULL;
}

int emptyq(queue *q)
{
	return q->front == q->rear;
}

char getTopq(queue *q)
{
	if (emptyq(q))
	{
		printf("queue is empty!\n");
		return -1;
	}
	return q->front->next->data; 
}

int enqueue(queue *q,elemtype data)
{
	Qnode *p = (Qnode *)malloc(sizeof(Qnode));
	if (p == NULL)
	{
		printf("error\n");
		return -1;
	}
	p->data = data;
	p->next = NULL;
	q->rear->next = p;
	q->rear = q->rear->next;
	return 0;
}

int outqueue(queue *q)
{
	if (emptyq(q))
	{
		printf("queue is empty!\n");
		return -1;
	}
	Qnode *p = q->front->next;
	q->front->next = p->next;
	if (p->next == NULL)
	{
		q->rear = q->front;
	}
	free(p);
	return 0;
}

typedef struct node
{
	elemtype data;
	struct node *next;
}stack;

void initstack(stack **s)
{
	(*s) = (stack *)malloc(sizeof(stack));
	(*s)->next = NULL;
}

int emptys(stack **s)
{
	return (*s)->next == NULL;
}

char getTops(stack **s)
{
	if (emptys(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	return (*s)->next->data;
}

int push(stack **s,elemtype data)
{
	stack *p = (stack *)malloc(sizeof(stack));
	if (p == NULL)
	{
		printf("error\n");
		return -1;
	}
	p->next = (*s)->next;
	p->data = data;
	(*s)->next = p;
	return 0;
}

int pop(stack **s)
{
	if (emptys(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	stack *p = (*s)->next;
	(*s)->next = p->next;
	free(p);
	return 1;
}

void main()
{
	int i, flag = 0;
	char a[100];
	printf("input string:\n");
	scanf("%s",a);
	stack *s;
	queue q;
	initstack(&s);
	initqueue(&q);
	for (i = 0; i < strlen(a)/2; i++)
	{
		enqueue(&q,a[i]);
	}
	for (i = strlen(a)/2+strlen(a)%2; a[i] != '\0'; i++)
	{
		push(&s,a[i]);
	}
	while(!emptys(&s))
	{
		printf("%c",getTopq(&q));
		printf("%c\n",getTops(&s));
		if (getTops(&s) != getTopq(&q))
		{
			flag = 1;
		}
		outqueue(&q);
		pop(&s);
	}
	if (flag)
	{
		printf("no\n");
	}
	else
	{
		printf("yes\n");
	}
}
