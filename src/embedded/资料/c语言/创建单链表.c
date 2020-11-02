#include <stdio.h>
#include <malloc.h>

typedef struct node
{
	int data;
	struct node *next;
}Node;

Node *create_node(int data)
{
	Node *new_node;
	new_node = (Node *)malloc(sizeof(Node));
	new_node->data = data;
	new_node->next = NULL;
	return new_node;
}

Node insert_node(Node head, Node *new_node)
{
	if (new_node == NULL)
	{
		printf("oreate node is NULL\n");
		return head;
	}
	if (head.next != NULL)
	{
		new_node->next = head.next;
	}
	head.next = new_node;
	return head;
}

void output(Node head)
{
	Node *p = head.next;
	for (; p != NULL; p = p->next)
	{
		printf("%d",p->data);
	}
	printf("\n");
}

void outdestroy(Node head)
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
		head = insert_node(head,create_node(n));
	}
	output(head);
	outdestroy(head);
}
