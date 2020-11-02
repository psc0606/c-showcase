#include<stdio.h>
int main()
{
	int n,m=0;
	scanf("%d",&n);
	while(n!=0)
	{
		m++;
		printf("%d",n%10);
		n=n/10;
	}
	printf("\n");
	printf("它是%d位数",m);
}
