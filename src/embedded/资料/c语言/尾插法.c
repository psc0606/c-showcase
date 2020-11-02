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
	int i, n;
	Node head;
	head.data = 0;
	head.next = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	output(head);
	destroy(head);
}
