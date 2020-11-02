#include<stdio.h>
#include<stdlib.h>

typedef struct Node
{
	struct Node* left;
	int value;
	struct Node* right;
}node;

node* create(int value)
{
	node* new=(node*)malloc(sizeof(node));
	new->left=NULL;
	new->value=value;
	new->right=NULL;
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
		new->right=temp_head;
		temp_head=new;
	}
	*head=temp_head;
	return 1;
}

/*int del_node(node* *head,int value)
{
	node* temp_head=*head;
	node *p;
	if(temp_head==NULL)
	{
		printf("the link is null.\n");
		return 0;
	}
	for(p=temp_head;p!=NULL;p=p->right)
	{
		if(p->value==value)
			break;
	}
	if(p==temp_head)
	{
		p->right->left=NULL;
		temp_head=p->right;
		//free(p);
	}
	else if(p==NULL)
	{
		printf("不存在.\n");
		return 0;
	}
	else if(p->right==NULL)
	{
		p->left->right=NULL;
		free(p);
	}
	else
	{
		p->left->right=p->right;
		p->right->left=p->left;
		free(p);
	}
	*head=temp_head;
	return 1;
}*/

int del_node(node* *head,int value)
{
	node* temp_head=*head;
	node* p;
	if(NULL==temp_head)
	{
		printf("the link is null.\n");
		return 0;
	}
	for(p=temp_head;p!=NULL;p=p->right)
	{
		if(p->value==value)
		{
			break;
		}
	}
	if(NULL==p)
	{
		printf("the value is not found.\n");
		return 0;
	}
	else if(temp_head==p)
	{
		p->right->left=NULL;
		temp_head=p->right;
	}
	else if(p->right==NULL)
	{
		p->left->right=NULL;
		free(p);
	}
	//printf("p->left->value:%d",p->left->value);
	else
	{
		//printf("gagh");
		//printf("p->left->value:%d",p->left->value);
		p->left->right=p->right;
		p->right->left=p->left;
		free(p);
	}
	*head=temp_head;
	return 1;
}


void sort(node* head)
{
	node *p,*q;
	int t;
	for(p=head;p->right!=NULL;p=p->right)
	{
		for(q=p->right;q!=NULL;q=q->right)
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

void output(node* head)
{
	node* p;
	for(p=head;p!=NULL;p=p->right)
	{
		printf("%d\t",p->value);
	}
	printf("\n");
}

void destroy(node* head)
{
	node *p,*t;
	p=head;
	while(p!=NULL)
	{
		t=p;
		p=p->right;
		//printf("t->value=%d\n",t->value);
		//t->right=NULL;
		p->left=;
		//(*p).left=NULL;
		free(t);
	}
}

int main()
{
	int value;
	node* head=NULL;
	scanf("%d",&value);
	while(value!=-1)
	{
		if(insert_head(&head,create(value)))
		{
			printf("insert successful!\n");
		}
		scanf("%d",&value);
	}
	output(head);
	printf("the sorted numbers:\n");
	sort(head);
	output(head);
	/*printf("input value del.\n");
	scanf("%d",&value);
	if(del_node(&head,value))
	{
		printf("delate %d successful.\n",value);
	}
	//del_node(&head,2);
	output(head);*/
	destroy(head);
	//printf("head->value:%d\n",head->value);
	printf("after destroyed:\n");
	output(head);
}

