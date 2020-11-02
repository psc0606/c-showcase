#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node
{
	char name[20];
	int num;
	char sex;
	int age;
	struct Node *next;
}node;

node* create_node(char *name,int num,char sex,int age)
{
	node* new = (node*)malloc(sizeof(node));
	strcpy(new->name,name);
	new->num=num;
	new->sex=sex;
	new->age=age;
	new->next=NULL;
	return new;
} 

int insert_head(node* *head,node* new)
{
	if(new==NULL)
	{
		printf("the node is null.\n");
		return 0;
	}
	if((*head)!=NULL)
		new->next=*head;
	*head=new;
	return 1;
}

int del_node(node* *head,int age)
{
	node *p,*q;
	if(*head==NULL)
	{
		printf("the link is null.\n");
		return 0;
	}
	p=q=*head;
	for(;q!=NULL;p=q,q=q->next)
	{
		if(q->age==age)
			break;
	}
	if(q==*head)
	{
		printf("删除头结点.\n");
		*head=q->next;
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
	return 1;	
}

void output(node* head)
{
	node* p;
	for(p= head;p!= NULL; p=p->next )
		printf("%s\t%d\t%c\t%d\n",p->name,p->num,p->sex,p->age);
	printf("\n");
}

int main()
{
	node* head=NULL;
	char name[20];
	int num;
	char sex;
	int age;
	scanf("%s",name);
	scanf("%d",&num);
	getchar();
	scanf("%c",&sex);
	scanf("%d",&age);
	while(age!=-1)
	{
		if(insert_head(&head,create_node(name,num,sex,age)))
		{
			printf("insert successful.\n");
		}
		//scanf("%s,%d,%c,%d",name,&num,&sex,&age);
		scanf("%s",name);
		scanf("%d",&num);
		getchar();
		scanf("%c",&sex);
		scanf("%d",&age);
	}
	output(head);
	while(1)
	{
		printf("please input the age you want to delete:");
		scanf("%d",&age);
		if(del_node(&head,age))
		{
			printf("delete %d successful.\n",age);
		}
		else
		{
			break;	
		}
		output(head);
	}
}
