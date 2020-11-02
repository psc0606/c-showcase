/*----------------------------------------
**	1. 创建两个链表a，b，写一个函数，函数原型
**	如下：int Sub_link(Node* a, Node * b);
**	此函数要完成在链表a中查找与之匹配的链表b。
**----------------------------------------
*/

#include <stdio.h>
#include <malloc.h>

typedef struct node
{
	int data;
	struct node *next;
}Node;

Node *creat_node(int data)
{
	Node *p = (Node *)malloc(sizeof(Node));
	if (p != NULL)
	{
		p->data = data;
		p->next = NULL;
	}
	return p;
}

Node *insert_node(Node *head,Node *new_node)
{
	if (new_node == NULL)
	{
		printf("empty\n");
		return head;
	}
	if (head != NULL)
	{
		new_node->next = head;
	}
	head = new_node;
	return head;
}

int Sub_link(Node *a, Node *b)
{
	Node *p, *q, *t;
	for (p = a; p != NULL; p = p->next)
	{
		t = p;
		for (q = b; q != NULL; q = q->next)
		{
			if (t->data != q->data)
			{
				break;
			}
			else
			{
				t = t->next;
			}
		}
		if (q == NULL)
		{
			return 1;
		}
	}
	return 0;
}

Node destroy(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = head)
	{
		head = p->next;
		free(p);
	}
}

void main()
{
	int i, n;
	Node *a = NULL, *b = NULL;
	printf("input a:\n");
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		a = insert_node(a,creat_node(n));
	}
	printf("input b:\n");
	for (i = 0; i < 3; i++)
	{
		scanf("%d",&n);
		b = insert_node(b,creat_node(n));
	}
	if (Sub_link(a,b))
	{
		printf("a包含b\n");
	}
	else
	{
		printf("a不包含b\n");
	}
	destroy(a);
	destroy(b);
}
