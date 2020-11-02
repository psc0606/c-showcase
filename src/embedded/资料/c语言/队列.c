#include <stdio.h>
#define MaxSize 10

typedef int elemtype;
typedef struct
{
	elemtype data[MaxSize];
	int rear;
	int front;
}queue;

int empty(queue *s)
{
	return q->rear == q->front;
}

int is_full(queue *q)
{
	return (q->rear+1) % MaxSize == q->front;
}

void initqueue(queue *q)
{
	q->rear = q->front = 0;
}

int enqueue(queue *q,elemtype data)
{
	if (is_full(q))
	{
		printf("queue is full!\n");
		return -1;
	}
	q->rear == (q->front+1)%MaxSize;
	q->data[q->rear] = data;	//rear测试
	return 0;
}

int outqueue(queue *q,elemtype data)
{
	if (empty(q))
	{
		printf("queue is empty!\n");
		return -1;
	}
	data = q->data[(q->front+1)%MaxSize];
	q->front = (q->front+1)% MaxSize;
	return 0;
}

int getTop(queue *q,elemtype data)
{
	if (empty(q))
	{
		printf("queue is empty!\n");
		return -1;
	}
	data = q->data[(q->front+1)%MaxSize];
	return 0;
}


void main()
{

}
