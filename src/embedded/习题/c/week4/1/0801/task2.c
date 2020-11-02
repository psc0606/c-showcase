#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
	int value;
	struct Node *next;
}node;

node* create_node(int value)
{
	node* new=(node*)malloc(sizeof(node));
	new->value=value;
	new->next=NULL;
	return new;
}

int insert(node* *head,node* new)
{
	node* temp_head=*head;
	node *p,*q;
	if(new==NULL)
	{
		printf("the node is null.\n");
		return 0;
	}
	if(temp_head==NULL)
		temp_head=new;
	else
	{
		p=q=temp_head;
		for(;q!=NULL;p=q,q=q->next)
		{
			if(new->value<=q->value)
				break;
		}
		if(q==temp_head)
		{
			new->next=q;
			temp_head=new;
		}
		else
		{
			p->next=new;
			new->next=q;
		}
	}
	*head=temp_head;
}

void nixu(node* *head)
{
	node* temp_head=*head;
	node *p,*q,*r;
	p=temp_head;
	q=p->next;
	while(q)
	{
		r=q->next;
		q->next=p;
		p=q;
		q=r;
	}
	temp_head->next=NULL;
	temp_head=p;
	*head=temp_head;
}

void output(node* head)
{
	node* p;
	for(p=head;p!=NULL;p=p->next)
	{
		printf("%d\t",p->value);
	}
	printf("\n");
}

int main()
{
	node* head=NULL;
	int value;
	scanf("%d",&value);
	while(value!=-1)
	{
		if(insert(&head,create_node(value)))
		{
			printf("insert successful.\n");
		}
		scanf("%d",&value);
	}
	output(head);
	nixu(&head);
	output(head);
}
