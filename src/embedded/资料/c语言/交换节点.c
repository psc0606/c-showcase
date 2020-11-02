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
	Node *p = head, *q, *m, *n, *k;
	while (p)
	{
		q = p;
		for (k = p; k != NULL; k = k->bwd)
		{
			if (q->data > k->data)
			{
				q = k;
			}
		}
		if (q != p)
		{
			printf("*");
			if (p->bwd == q)
			{
				if (p == head && q->bwd == NULL)
				{
					head = q;
					q->bwd = p;
					p->bwd = NULL;
					q->fwd = NULL;
					p->fwd = q;
				}
				else if (p == head)
				{
					head = q;
					p->bwd = q->bwd;
					q->bwd = p;
					p->bwd->fwd = p;
					q->fwd = NULL;
				}
				else if (q->bwd == NULL)
				{
					p->fwd->bwd = q;
					p->bwd = NULL;
					q->bwd = p;
					q->fwd = p->fwd;
					p->fwd = q;
				}
				else
				{
					p->fwd->bwd = q;
					p->bwd = q->bwd;
					q->bwd = p;
					p->bwd->fwd = p;
					q->fwd = p->fwd;
					p->fwd = q;
				}
				
			}
			else
			{
				m = p->bwd;
				n = q->fwd;
				if (p == head && q->bwd != NULL)
				{
					p->bwd = q->bwd;
					n->bwd = p;
					q->bwd = m;
					p->bwd->fwd = p;
					q->fwd = NULL;
					m->fwd = q;
					head = q;
					p = head;
				}
				else if (p == head && q->bwd == NULL)
				{
					p->bwd = NULL;
					n->bwd = p;
					q->bwd = m;
					q->fwd = NULL;
					m->fwd = q;
					p->fwd = n;
					head = q;
					p = head;
				}
				else if (p != head && q->bwd == NULL)
				{
					p->fwd->bwd = q;
					p->bwd = NULL;
					n->bwd = p;
					q->bwd = m;
					q->fwd = p->fwd;
					m->fwd = q;
					p->fwd = n;
				}
				else
				{
					p->fwd->bwd = q;
					p->bwd = q->bwd;
					n->bwd = p;
					q->bwd = m;
					p->bwd->fwd = p;
					q->fwd = p->fwd;
					m->fwd = q;
					p->fwd = n;
				}
			}
		}
		else
		{
			p = p->bwd;
		}
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
	int i, n;
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
