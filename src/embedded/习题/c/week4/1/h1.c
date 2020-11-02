/*----------------------------------------
**	1. 用尾插法插入节点，并且实现链表的基本
**	操作（创建，插入，删除，排序）；
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
	p->data = data;
	p->next = NULL;
	return p;
}

Node insert_node(Node head,Node *new_node)
{
	Node *tail;
	if (new_node == NULL)
	{
		printf("oreate node is NULL\n");
		return head;
	}
	if (head.next == NULL)
	{
		head.next = new_node;
	}
	else
	{
		for (tail = head.next;tail->next != NULL;tail=tail->next)
		{
		}
		tail->next = new_node;
	}
	return head;
}

Node sort(Node head)
{
	Node *p, *p1, *p2;
	int temp;
	for (p = head.next;p->next != NULL; p = p->next)
	{
		p1 = p;
		for (p2 = p->next; p2 != NULL; p2 = p2->next)
		{
			if (p1->data > p2->data)
			{
				p1 = p2;
			}
		}
		if (p1 != p)
		{
			temp = p1->data;
			p1->data = p->data;
			p->data = temp;
		}
	}
	return head;
}

Node delete_node(Node head,int data)
{
	Node *p, *q;
	for (p = head.next; p != NULL; p = p->next)
	{
		if (p->data == data)
		{
			break;
		}
		q = p;
	}
	if (p == NULL)
	{
		printf("no find in node\n");
		return head;
	}
	if (p == head.next)
	{
		head.next = p->next;
	}
	else
	{
		q->next = p->next;
	}
	free(p);
	return head;
}

Node insert_new(Node head,Node *new_node)
{
	if (creat_node == NULL)
	{
		printf("empty\n");
		return head;
	}
	Node *p, *q;
	for (q = head.next;q != NULL;q = q->next)
	{
		if (q->data > new_node->data)
		{
			break;
		}
		p = q;
	}
	if (head.next == q)
	{
		head.next = new_node;
		new_node->next = q;
	}
	else if (NULL == q)
	{
		p->next = new_node;
	}
	else
	{
		p->next = new_node;
		new_node->next = q;
	}
	return head;
}

void output(Node head)
{
	Node *p = head.next;
	for (;p != NULL; p = p->next)
	{
		printf("%d",p->data);
	}
	printf("\n");
}

void destroy(Node head)
{
	Node *p = head.next;
	for (; p != NULL; p = head.next)
	{
		head.next = p->next;
		free(p);
	}
}
void main()
{
	int i, n, m, l;
	Node head;
//	head.data = 0;
	head.next = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	output(head);
	head = sort(head);
	output(head);
	printf("输入要删除的值\n");
	scanf("%d",&m);
	head = delete_node(head,m);
	output(head);
//	printf("输入要插入的值\n");
//	scanf("%d",&l);
//	head = insert_new(head,creat_node(l));
//	output(head);
	destroy(head);
}
