 #include <stdio.h>
#include <malloc.h>
#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
	int data;
	struct node *next;
}NODE;

NODE *create_node (int data)
{
	NODE *new_node = (NODE *) malloc (sizeof (NODE));
	if (new_node != NULL)
	{
		new_node->data = data;
		new_node->next = NULL;
	}
	return new_node;
}

int insert_node (NODE **head, NODE *new_node)
{
	if (new_node == NULL)
	{
		printf ("create node is NULL.\n");
		return FAILURE;
	}
	if (*head == NULL)
	{
		*head = new_node;
		return SUCCESS;		
	}  
	if (*head != NULL)
	{
	//	new_node->next = (*head)->next;    
      new_node->next = *head;         //  不断变化的，头结点插法			 
	} 
	//(*head)->next = new_node;
  	*head = new_node;
	return SUCCESS;
}

void output (NODE *p)
{
	while (p != NULL)
	{
		printf ("%d ", p->data);
		p = p->next;
	} 
	printf ("\n");
}

int Sub_link(NODE* a, NODE * b)
{
	NODE *p1 = a, *p2;
	while (p1 != NULL)
	{		
		p2 = b;
		while (p2 != NULL)
		{
			if (p2->data != p1->data)
			{
				break;
			}
			p1 = p1->next;
			p2 = p2->next;
		}
		if (p2 == NULL)
		{
			return 1;
		}
		p1 = p1->next;
	}
	return 0;
}

int main ()
{
	NODE *head[2];
	int i, m, data;
	
	for (i = 0; i < 2; i++)
	{
		head[i] = NULL;
		printf ("输入链表%d长度：", i);
		scanf ("%d", &m);
		printf ("输入链表%d：", i);
		while (m)							//头插法，始终插在head后面
		{
			scanf ("%d", &data);
			if (FAILURE == insert_node (&head[i], create_node (data)))
			{
				printf ("Error, fail to estabilsh a link table!");
			}
			m--;
		}
	}
	
	for (i = 0; i < 2; i++)
	{
		printf ("输出链表%d：", i);
		output (head[i]);
	}
	if (Sub_link (head[0], head[1]))
	{
		printf ("存在匹配的！\n");
	}
	else
	{
		printf ("不存在匹配的！\n");		
	}
}
