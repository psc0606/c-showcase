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

Node insert_node(Node head, Node *new_node)
{
	Node *p = head.next;
	if (new_node == NULL)
	{
		printf("new_node error\n");
		return head;
	}
	if (head.next == NULL)
	{
		head.next = new_node;
	}
	else
	{
		for (;p->next != NULL; p = p->next)		//注意这里判断next为NULL!
		{
		}
		p->next = new_node;
	}
	return head;
}

Node sort_node(Node head)
{
	Node *p, *q, *m;
	int temp;
	for (p = head.next; p != NULL; p = p->next)
	{
		q = p;
		for (m = p->next; m != NULL; m = m->next)
		{
			if (m->data < q->data)
			{
				q = m;
			}
		}
		if (q != p)
		{
			temp = p->data;
			p->data = q->data;
			q->data = temp;
		}
	}
	return head;
}

Node delete_node(Node head,int data)
{
	Node *q, *p = head.next;
	for (; p != NULL; p = p->next)
	{
		if (p->data == data)
		{
			break;
		}
		q = p;
	}
	if (p == NULL)
	{
		printf("no find\n");
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

Node insert_new(Node head,int data)
{
	Node *p, *q, *new = creat_node(data);
	for (p = head.next,q = p->next; q != NULL; p = p->next,q = p->next)
	{
		if (p->data < data && q->data > data)
		{
			new->data = data;
			p->next = new;
			new->next = q;
			return head;
		}
	}
	q = head.next;
	if (p->data > data)
	{
		head.next = new;
		new->next = q;
	}
	else
	{
		p->next = new;
	}
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
	for (;p != NULL; p = head.next)
	{
		head.next = p->next;
		free(p);
	}	
}

void main()
{
	int i, n, a, b;
	Node head;
	head.data = 0;
	head.next = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	display(head);
	head = sort_node(head);
	display(head);
	printf("del:\n");
	scanf("%d",&a);
	head = delete_node(head,a);
	display(head);
	printf("insert data\n");
	scanf("%d",&b);
	head = insert_new(head,b);
	display(head);
	destroy(head);
}
