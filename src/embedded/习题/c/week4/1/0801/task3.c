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
	if(NULL==new)
	{
		printf("create a node failed.\n");
		return new;
	}
	new->value=value;
	new->next=NULL;
}

int insert_head(node* *head,node* new_node)
{
	node* temp_head=*head;
	if(new_node==NULL)
	{
		printf("the node is null.\n");
		return 0;
	}
	if(temp_head==NULL)
	{
		temp_head=new_node;
	}
	else
	{
		node* p;
		for(p=temp_head;p->next!=NULL;p=p->next);
		p->next=new_node;
	}
	*head=temp_head;
	return 1;
}

int Sub_link(node* ahead,node* bhead)
{
	node *p,*q,*t;
	for(p=ahead;p!=NULL;p=p->next)
	{
		t=p;
		for(q=bhead;q!=NULL;q=q->next)
		{
			if(q->value!=t->value)
			{
				break;
			}
			else
			{
				t=t->next;
			}
		}
		if(q==NULL)
			return 1;
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
	node* ahead=NULL;
	node* bhead=NULL;
	int value;
	printf("please insert link a:\n");
	scanf("%d",&value);
	while(value!=-1)
	{
		if(insert_head(&ahead,create_node(value)))
		{
			printf("insert successful.\n");
		}
		scanf("%d",&value);
	}
	output(ahead);
	printf("please insert link b:\n");
	scanf("%d",&value);
	while(value!=-1)
	{
		if(insert_head(&bhead,create_node(value)))
		{
			printf("insert successful.\n");
		}
		scanf("%d",&value);
	}
	output(bhead);
	if(Sub_link(ahead,bhead))
		printf("匹配\n");
	else
		printf("不匹配\n");
}
