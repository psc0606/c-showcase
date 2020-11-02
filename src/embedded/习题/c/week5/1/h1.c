/*----------------------------------------
**	1. 实现链式队列的基本操作。
**----------------------------------------
*/
/*
顺序:
#include <stdio.h>
#include <malloc.h>
#define MaxSize 5

typedef int elemtype;
typedef struct
{
	elemtype data[MaxSize];
	int front,rear;
}queue;

void initqueue(queue *q)
{
	q->front = q->rear = 0;
}

int empty(queue *q)
{
	return q->front == q->rear;
}

int is_full(queue *q)
{
	return (q->rear+1)%MaxSize == q->front;
}

int enqueue(queue *q,elemtype data)
{
	if (is_full(q))
	{
		printf("queue is full!\n");
		return -1;
	}
	q->rear = (q->rear+1)%MaxSize;
	q->data[q->rear] = data;
	return 0;
}

int outqueue(queue *q,elemtype data)
{
	if (empty(q))
	{
		printf("queue is empty!\n");
		return -1;
	}
	data = q->data[(q->front+1) % MaxSize];
	q->front = (q->front+1) % MaxSize;
	return 0;
}

int getTop(queue *q)
{
	return q->data[(q->front+1) % MaxSize];
}

void main()
{
	int i, n, m;
//	queue q;
	queue *q = (queue *)malloc(sizeof(queue));
	initqueue(q);
	for (i = 0; i < 4; i++)
	{
		scanf("%d",&n);
		enqueue(q,n);
	}
	while(!empty(q))
	{
		printf("%d",getTop(q));
		outqueue(q,m);
	}
	free(q);
}
*/

//链式:
#include <stdio.h>
#include <malloc.h>

typedef int elemtype;
typedef struct qnode
{
	elemtype data;
	struct qnode *next;
}Qnode;

typedef struct
{
	Qnode *front;
	Qnode *rear;
}queue;

void initqueue(queue *q)
{
	q->front = q->rear = (Qnode *)malloc(sizeof(Qnode));		//注意！
	q->front->next = NULL;
}

int empty(queue *q)
{
	return q->front == q->rear;
}

int getTop(queue *q,elemtype *data)
{
	if (empty(q))
	{
		printf("queue is empty!\n");
		return -1;
	}
	*data = q->front->next->data;
	return 0;
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
//	p->next = q->rear->next;
	q->rear->next = p;
	q->rear = q->rear->next;
	return 0;
}

int outqueue(queue *q)
{
	if (empty(q))
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

void main()
{
	int i, n, m;
	queue q;
	initqueue(&q);
	for (i = 0; i < 4; i++)
	{
		scanf("%d",&n);
		enqueue(&q,n);
	}
	while(!empty(&q))
	{
		getTop(&q,&m);
		printf("%d",m);
		outqueue(&q);
	}
}
