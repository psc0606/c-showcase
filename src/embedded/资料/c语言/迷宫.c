/*----------------------------------------
**	3. 利用栈求解迷宫，已知迷宫的入口为右下角，
**	出口为左上角。
	0	0	0	1	1	1
	0	1	0	1	0	1
	1	0	0	1	0	1
	1	0	1	0	0	0
	0	0	0	0	1	0
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

typedef int elemtype;

struct
{
	int i;
	int j;
}dir[4] = {{-1,0},{0,1},{1,0},{0,-1}};

typedef struct node
{
	elemtype i, j, d;
	struct node *next;
}stack;

void initstack(stack **s)
{
	*s = (stack *)malloc(sizeof(stack));
	(*s)->next = NULL;
}

int empty(stack **s)
{
	return (*s)->next == NULL;
}

int getTop(stack **s,int *i,int *j,int *d)
{
	*i = (*s)->next->i;
	*j = (*s)->next->j;
	*d = (*s)->next->d;
	return 0;
}

int push(stack **s,elemtype i,elemtype j,elemtype d)
{
	stack *p = (stack *)malloc(sizeof(stack));
	if (p == NULL)
	{
		printf("error!\n");
		return -1;
	}
	p->i = i;
	p->j = j;
	p->d = d;
	p->next = (*s)->next;
	(*s)->next = p;
	return 0;
}

int pop(stack **s,elemtype *i,elemtype *j,elemtype *d)
{
	if (empty(s))
	{
		printf("stack is empty!\n");
		return -1;
	}
	*i = (*s)->next->i;
	*j = (*s)->next->j;
	*d = (*s)->next->d;
	stack *p = (*s)->next;
	(*s)->next = p->next;
	free(p);
	return 1;
}

int search(stack *s,int a[7][8], int i, int j)
{
	int k = 0;
	while (1)
	{
		if (a[i][j] != 1)
		{
			a[i][j] = 2;
			if (i == 1 && j == 1)
			{
				break;
			}	
			while (k < 4)
			{
				if (a[i+dir[k].i][j+dir[k].j] == 0)
				{
					push(&s,i,j,k);
					i = i+dir[k].i;
					j = j+dir[k].j;
					k = 0;
					break;
				}
				else
				{
					k++;
				}
			}
			if (k >= 4)
			{
				if (empty(&s))
				{
					printf("no find");
					return ;
				}
				pop(&s,&i,&j,&k);
				k++;
			}
		}
	}
}

void main()
{
	int a[7][8] = {
					1,1,1,1,1,1,1,1,
					1,0,0,0,1,1,1,1,
				   	1,0,1,0,1,0,1,1,
				    1,1,0,0,1,0,1,1,
				   	1,1,0,1,0,0,0,1,
				   	1,0,0,0,0,1,0,1,
				   	1,1,1,1,1,1,1,1};
	int i, j, k;
	stack *s;
	initstack(&s);
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%3d",a[i][j]);
		}
		printf("\n");
	}
	
	search(s,a,5,6);
	
	printf("*****************************\n");
	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%3d",a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	while (!empty(&s))
	{
		pop(&s,&i,&j,&k);
		printf("i = %d,j = %d\n",i,j);
	}
	free(s);
}
