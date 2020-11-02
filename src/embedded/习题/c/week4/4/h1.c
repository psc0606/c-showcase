/*----------------------------------------
**	1. 已知两个链表head1和head2各自有序，请
**	把他们合并成一个链表依然有序。
**	（保留所有节点，即便大小相同）。
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
		while (p->bwd)
		{
			p = p->bwd;
		}
		p->bwd = new_node;
		new_node->fwd = p;
	}
	return head;
}

Node *sort(Node *head1, Node *head2)
{
	Node *p = head1, *q, *m;
	while (q = head2)
	{
		head2 = q->bwd;
		m = NULL;
		for (p = head1; p != NULL; p = p->bwd)
		{
			if (q->data <= p->data)
			{
				break;
			}
			m = p;
		}
		if (m == NULL)
		{
			q->bwd = head1;
			head1 = q;
			q->fwd = NULL;
		}
		else if (p == NULL)
		{
			m->bwd = q;
			q->fwd = m;
			q->bwd = NULL;
		}
		else
		{
			m->bwd = q;
			q->fwd = m;
			q->bwd = p;
			p->fwd = q;
		}
	}
	return head1;
}

void display(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = p->bwd)
	{
		printf("%3d",p->data);
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
	int i, n;
	Node *head1 = NULL, *head2 = NULL;
	printf("input head1:\n");
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head1 = insert_node(head1,creat_node(n));
	}
	display(head1);
	printf("input head2:\n");
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head2 = insert_node(head2,creat_node(n));
	}
	display(head2);
	head1 = sort(head1,head2);
	display(head1);
	destroy(head1);
	destroy(head2);
}
