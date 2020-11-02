/*----------------------------------------
**	2. 使用排序插入法创建一个链表，编写一个
**	函数，逆序排列一个单链表所有节点。
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

Node *sort(Node *head)
{
	Node *p, *q;
	if (head && head->next)
	{
		p = head;
		q = p->next;
		p->next = NULL;
		while (q)				//算法很经典！
		{
			p = q;
			q = q->next;
			p->next = head;
			head = p;
		}
	}
	return head;
}

void display(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = p->next)
	{
		printf("%d",p->data);
	}
	printf("\n");
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
	Node *head = NULL;
	for (i = 0; i < 5; i++)
	{
		scanf("%d",&n);
		head = insert_node(head,creat_node(n));
	}
	display(head);
	head = sort(head);
	display(head);
	destroy(head);
}
