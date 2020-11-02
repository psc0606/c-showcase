/*----------------------------------------
**	1. 实现双链表的反序排列。
**----------------------------------------
*/

/*----------------------------------------
**	1. 分别用头插和尾插法插入节点，并且实现双
**	链表的基本操作(创建，插入，删除，排序，销毁)
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>

typedef struct node
{
	struct node *fwd;
	int data;
	struct node *bwd;
}Node;

Node *creat_node(int data)
{
	Node *p = (Node *)malloc(sizeof(Node));
	p->fwd = NULL;
	p->data = data;
	p->bwd = NULL;
}
/*
Node *insert_node(Node *head,Node *new_node)
{
	if (new_node == NULL)
	{
		printf("empty\n");
		return head;
	}
	if (head != NULL)
	{
		new_node->bwd = head;
		new_node->bwd->fwd = new_node;
	}
	head = new_node;
	return head;
}
*/
Node *insert_node(Node *head, Node *new_node)
{
	if (new_node == NULL)
	{
		printf("empty\n");
		return head;
	}
	if (head == NULL)
	{
		head = new_node;
	}
	else
	{
		Node *p = head;
		for (; p->bwd != NULL; p = p->bwd)
		{}
		p->bwd = new_node;
		new_node->fwd = p;
	}
	return head;
}

Node *sort_node(Node *head)
{
	Node *p, *q;
	if (head && head->bwd)
	{
		p = head;
		q = p->bwd;
		p->bwd = NULL;
		while (q)
		{
			p->fwd = q;
			p = q;
			q = q->bwd;
			p->bwd = head;
			head = p;
		}
		p->fwd = NULL;
	}
	return head;
}

void display(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = p->bwd)
	{
		printf("%d",p->data);
	}
	printf("\n");
	for (p = head; p->bwd!= NULL; p = p->bwd)
	{
	}
	for (; p != NULL; p = p->fwd)
	{
		printf("%d",p->data);
	}
	printf("\n");
}

void destroy(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = head)
	{
		head = p->bwd;
		free(p);
	}
}

void main()
{
	int i, n, del;
	Node *head = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	display(head);
	head = sort_node(head);
	display(head);
	destroy(head);
}
