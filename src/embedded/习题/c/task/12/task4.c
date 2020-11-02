#include<stdio.h>
void main()
{
	void sort(int **p,int n);
	int i,n,digit[20],**p,*pstr[20];
	printf("input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
		pstr[i]=&digit[i];
	printf("input %d integer numbers:\n",n);
	for(i=0;i<n;i++)
		scanf("%d",pstr[i]);
	p=pstr;
	sort(p,n);
	printf("Now,the sequence is:\n");
	for(i=0;i<n;i++)
		printf("%d  ",*pstr[i]);
	printf("\n");
}

void sort(int **p,int n)
{
	int i,j,*temp;
	for(i=0;i<n-1;i++)
	{
		for(j=i+1;j<n;j++)
		{
			if(**(p+i)>**(p+j))
			{
				temp=*(p+i);
				*(p+i)=*(p+j);
				*(p+j)=temp;	
			}
		}
	}
}
