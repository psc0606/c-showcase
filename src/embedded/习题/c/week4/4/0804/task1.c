#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
	int value;
	struct Node* next;
}node;

node* create(int value)
{
	node* new=(node*)malloc(sizeof(node));
	new->value=value;
	new->next=NULL;
	return new;
}

int insert(node* *head,node* new)
{
	node *p,*q;
	if(new==NULL)
	{
		printf("the node is null.\n");
		return 0;
	}
	for(p=q=*head;q!=NULL;p=q,q=q->next)
	{
		if(q->value>new->value)
			break;
	}
	if(q==*head)
	{
		new->next=q;
		*head=new;
	}
	else if(q==NULL)
	{
		p->next=new;
	}
	else
	{
		p->next=new;
		new->next=q;
	}
	return 1;
}

node* Merge(node* ahead,node* bhead)
{
	if(ahead==NULL)
		return bhead;
	if(bhead==NULL)
		return ahead;
	node* head=NULL;
	node* p1;
	node* p2;
	if(ahead->value<bhead->value)
	{
		head=ahead;
		p1=ahead->next;
		p2=bhead;
	}
	else
	{
		head=bhead;
		p1=ahead;
		p2=bhead->next;
	}
	node* p=head;
	while(p1!=NULL && p2!=NULL)
	{
		if(p1->value<p2->value)
		{
			p->next=p1;
			p1=p1->next;
			p=p->next;
		}
		else
		{
			p->next=p2;
			p2=p2->next;
			p=p->next;
		}
	}
	if(p1!=NULL)
		p->next=p1;
	if(p2!=NULL)
		p->next=p2;
	return head;
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
	node *ahead=NULL,*bhead=NULL,*head;
	int value;
	printf("link a:\n");
	scanf("%d",&value);
	while(value!=-1)
	{
		if(insert(&ahead,create(value)))
		{
			printf("insert successful!\n");
		}
		scanf("%d",&value);
	}
	output(ahead);
	printf("link b:\n");
	scanf("%d",&value);
	while(value!=-1)
	{
		if(insert(&bhead,create(value)))
		{
			printf("insert successful!\n");
		}
		scanf("%d",&value);
	}
	output(bhead);
	head=Merge(ahead,bhead);
	printf("a和b合并:\n");
	output(head);
}

