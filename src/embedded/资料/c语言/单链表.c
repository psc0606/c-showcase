#include<stdio.h>
#include<malloc.h>

typedef struct node
{
	int data;
	struct node * next;
}Node;

Node* create_node(int data)
{
	Node* new_node= (Node*)malloc(sizeof(Node));
	if(new_node != NULL)
	{
		new_node->data = data;
		new_node->next = NULL;
	}
	return new_node;

}

void destory(Node head)
{
	Node* p; 
	for(p = head.next;p!= NULL;p=head.next)
	{
		head.next = p->next;
		free(p);
	}
}

Node sort(Node head)
{
	Node *p,*p1,*p2;
	int temp;
	for(p =head.next;p->next != NULL;p=p->next)
	{
		p1 = p;
		for(p2 = p->next; p2 != NULL;p2=p2->next)
		{
			if(p1->data > p2->data)
				p1 = p2;
		}
		if(p1 != p)
		{
			temp = p1->data;
			p1->data = p->data;
			p->data = temp;
		}
	}
	return head;
}

Node delete_node(Node head,int data)
{
	Node* p,*q;
	
	for(p=head.next; p!= NULL;p=p->next)
	{
		if(p->data == data)
			break;
		q = p;
	}
	if(p == NULL)
	{
		printf("no find node :%d\n",data);
		return head;
	}
	
	if(p == head.next)
	{
		head.next = p->next;
	}
	else
	{
		q->next = p->next;
	}
	free(p);
	return head;
	
}



Node insert_sort(Node head,Node* new_node)
{
	if(new_node == NULL)
	{
		printf("the new node is empty.\n");
		return head;
	}
	Node *p,*q;
	for(q= head.next;q!= NULL;q=q->next)
	{
		if(q->data > new_node->data)
			break;
		p = q;
	}
	if(head.next == q )
	{
		head.next = new_node;
		new_node->next = q;
	}
	else if( NULL == q)
	{
		p->next = new_node;
	}
	else
	{
		p->next = new_node;
		new_node->next = q;
	}
	return head;
	
}



















Node insert_node(Node head,Node* new_node)
{
	Node* tail;
	if(NULL == new_node)
	{
		printf("insert failed,the new node is NULL\n");
		return head;
	}
	if(head.next == NULL)
	{
		head.next = new_node;
	}
	else
	{
		for(tail= head.next;tail->next!=NULL;tail= tail->next);
		tail->next = new_node;
	}
	return head;
}

void output(Node head)
{
	Node* p = head.next;
	for(;p!= NULL;p=p->next)
		printf("%3d ",p->data);
	printf("\n");
}
int main()
{
	int i,data;
	Node head;
	head.data =0;
	head.next = NULL;
	for(i=0;i<5;i++)
	{
		scanf("%d",&data);
		head = insert_sort(head,create_node(data));
	}
	output(head);
	//sort(head);
	//output(head);
	printf("input a value del:");
	scanf("%d",&data);
	head =delete_node(head,data);
	output(head);
}

