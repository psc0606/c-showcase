/*----------------------------------------
**	3. 有n个人围成一圈，顺序排号。从第1个人开
**	始报数（从1 到3报数），凡报到3的人退出圈子，
**	问最后留下的是原来的第几号的那位。
**	（使用链表实现,提示:这n个人构成一个环形的链表）。
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

void destroy(Node *head)
{
	int i = 0;
	Node *p = head, *q;
	for (;p != NULL; p = q->next)
	{
		i++;
		if (p == q)
		{
			printf("%3d",p->data);
			free(p);
			break;
		}
		if (i % 3 == 0)
		{
			q->next = p->next;
			printf("%3d",p->data);
			free(p);
			continue;
		}
		q = p;
	}
}

void main()
{
	int i, n;
	Node *head = NULL;
	printf("input n =");
	scanf("%d",&n);
	for (i = 1; i <= n; i++)
	{
		head = insert_node(head,creat_node(i));
	}
	Node *p = head;
	while (p->next)
	{
		p = p->next;
	}
	p->next = head;
	destroy(head);
}
