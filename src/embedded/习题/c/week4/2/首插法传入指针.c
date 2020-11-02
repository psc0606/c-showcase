
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
		printf("empty\n");
		return head;
	}
	if (head != NULL)
	{
		new_node->next = head;	//注意head!!
	}
	head = new_node;			//注意head!!
	return head;
}

Node *sort_node(Node *head)
{
	Node *p, *q, *m;
	int temp;
	for (p = head; p != NULL; p = p->next)
	{
		q = p;
		for (m = p->next; m != NULL; m = m->next)
		{
			if (q->data > m->data)
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

void display(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = p->next)
	{
		printf("%d",p->data);
	}
	printf("\n");
}

void destroy(Node *head)
{
	Node *p = head;
	for (; p != NULL; p = head->next)
	{
		head->next = p->next;
		free(p);
	}
}

void main()
{
	Node *head = NULL;
	int i, n;
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


/*
#include<stdio.h>
#include<malloc.h>

typedef struct Node
{
	int data;
	struct Node* next;
}Node;

Node* create_node(int data)
{
	Node* new_node = (Node*)malloc(sizeof(Node));
	if(new_node != NULL)
	{
		new_node->data = data;
		new_node->next = NULL;
	}
	return new_node;
}

Node* insert_node(Node* head,Node* new_node)
{
	if(new_node == NULL)
	{
		printf("the new node is empty.\n");
		return head;
	}
	if(head != NULL)
		new_node->next = head;
	head = new_node;
	return head;
}

Node* delete_node(Node* head,int data)
{
	Node *p,*q;
	for(q = head; q !=NULL;q = q->next )
	{
		if(q->data ==  data)
			break;
		p = q;
	}
	if(q == head)
	{
		head =  q->next;
	}
	else if(q == NULL)
	{
		printf("no find node :%d",data);
		return head;
	}
	else
	{
		p->next = q->next;
	}
	free(q);
	return head;
}

void output(Node* head)
{
	Node* p;
	for(p= head;p!= NULL; p=p->next )
		printf("%3d ",p->data);
	printf("\n");
}

void destory(Node* head)
{
	Node* p;
	for(p=head;p!= NULL;)
	{
		head= p->next;
		free(p);
		p = head;
	}
}

int main()
{
	Node* head = NULL;
	int data;
	int i;
	for(i=0;i<5;i++)
	{
		scanf("%d",&data);
		head = insert_node(head,create_node(data));
	}
	output(head);
	printf("del:");
	scanf("%d",&data);
	head= delete_node(head,data);
	output(head);
	destory(head);
	
}

*/
