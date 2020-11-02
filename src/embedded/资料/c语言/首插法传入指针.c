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
	for(p = head; p !=NULL;p = p->next )
	{
		if(p->data ==  data)
			break;
		q = p;
	}
	if(p == head)
	{
		head =  p->next;
	}
	else if(p == NULL)
	{
		printf("no find node :%d",data);
		return head;
	}
	else
	{
		q->next = p->next;
	}
	free(p);
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

