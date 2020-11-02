/*----------------------------------------
**	2. 判断单链表中是否存在环.
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

Node *insert_node(Node *head, Node *new_node)
{
	if (new_node == NULL)
	{
		printf("new_node is empty\n");
		return head;
	}
	if (head == NULL)
	{
		head = new_node;
	}
	else
	{
		Node *p = head;
		while (p->next)
		{
			p = p->next;
		}
		p->next = new_node;
	}
	return head;
}

Node *huan(Node *head)
{
	Node *p = head;
	for (; p->next != NULL; p = p->next)
	{}
	p->next = head->next;
	return head;
}

int fun(Node *head)
{
	Node *p = head->next, *q = head;
	while (p && q)
	{
		if (p == q)
		{
			return 1;
		}
		p = p->next->next;
		q = q->next;
	}
	return 0;
}

void display(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = p->next)
	{
		printf("%3d",p->data);
	}
	printf("\n");
}

void destroy(Node *head)
{
	Node *p = head, *q = head;
	for (; p != NULL; p = head,q = head->next->next)
	{
		if (p == q)
		{
			free(p);
			break;
		}
		head = p->next;
		free(p);
	}
}

void main()
{
	int i, n;
	Node *head = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	head = huan(head);
	if (fun(head))
	{
		printf("有环\n");
	}
	else
	{
		printf("无环\n");
	}
	destroy(head);
}
