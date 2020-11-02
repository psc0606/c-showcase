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
	node* p;
	if(new==NULL)
	{
		printf("the node is null");
		return 0;
	}
	if(*head!=NULL)
	{
		new->next=*head;
	}
	*head=new;
}

void addCircle(node* head,int n)
{
	node *q,*p=head;
	int i;
	for(i=1;p->next!=NULL;i++)
	{
		if(i==n) q=p;
		p=p->next;
	}
	p->next=q;
}

int isCircle(node* head)
{
	if(head==NULL) return 0;
	node *low=head,*fast=head->next;
	while(fast!=NULL && fast->next!=NULL)
	{
		low=low->next;
		fast=fast->next->next;
		if(low==fast) return 1;
	}
	return 0;
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
		if(insert(&head,create(value)))
			printf("insert successful.\n");
		scanf("%d",&value);
	}
	output(head);
	//addCircle(head,3);
	if(isCircle(head))
		printf("有环");
	else
		printf("无环");
}
