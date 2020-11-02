#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
	int value;
	struct Node *next;
}node;

node* create_node(int value)
{
	node* new = (node*)malloc(sizeof(node));
	new->value=value;
	new->next=NULL;
	return new;
} 

int insert_head(node* *head,node* new)
{
	node* temp_head=*head;
	if(new==NULL)
	{
		printf("the node is null.\n");
		return 0;
	}
	if(temp_head==NULL)
	{
		temp_head=new;
	} 	
	else
	{
		new->next=temp_head;
		temp_head=new;
	}
	*head=temp_head;
	return 1;
}

int insert_tail(node* *head,node* new)
{
	node* temp_head=*head;
	if(new==NULL)
	{
		printf("the node is null.\n");
		return 0;
	}
	if(temp_head==NULL)
	{
		temp_head=new;
	}
	else
	{
		node* p;
		for(p=temp_head;p->next!=NULL;p=p->next);
		p->next=new;
	}
	*head=temp_head;
	return 1;
}

int del_node(node* *head,int value)
{
	node* temp_head=*head;
	node *p,*q;
	if(temp_head==NULL)
	{
		printf("the link is null.\n");
		return 0;
	}
	p=q=temp_head;
	for(;q!=NULL;p=q,q=q->next)
	{
		if(q->value==value)
			break;
	}
	if(q==temp_head)
	{
		printf("删除头结点.\n");
		temp_head=q->next;
		free(q);
	}
	else if(q==NULL)
	{
		printf("不存在.\n");
		return 0;
	}
	else
	{
		p->next=q->next;
		free(q);
	}
	*head=temp_head;
	return 1;	
}

void sort(node *head)
{
	node *p,*q;
	int t;
	for(p=head;p->next!=NULL;p=p->next)
	{
		for(q=p->next;q!=NULL;q=q->next)
		{
			if(p->value>q->value)
			{
				t=p->value;
				p->value=q->value;
				q->value=t;
			}
		}
	}
}

void output(node *head)
{
	node *p;
	for(p=head;p!=NULL;p=p->next)
	{
		printf("%d\t",*p);
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
		if(insert_tail(&head,create_node(value)))
		{
			printf("insert successful.\n");
		}
		scanf("%d",&value);
	}
	output(head);
	printf("the sorted numbers.\n");
	sort(head);
	output(head);
	while(1)
	{
		printf("please input the value you want to delete:");
		scanf("%d",&value);
		if(del_node(&head,value))
		{
			printf("delete %d successful.\n");
		}
		else
		{
			break;	
		}
		output(head);
	}
}
