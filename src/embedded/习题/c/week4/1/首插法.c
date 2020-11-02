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

Node insert_node(Node head, Node *new_node)
{
	if (new_node == NULL)
	{
		printf("data is empty\n");
		return head;
	}
	if (head.next != NULL)		//技巧！
	{
		new_node->next = head.next;	//
	}
	head.next = new_node;
	return head;
}

void display(Node head)
{
	Node *p = head.next;
	for (; p != NULL; p = p->next)
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
	head.next = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	display(head);
	destroy(head);
}
