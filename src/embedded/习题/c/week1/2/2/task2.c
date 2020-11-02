#include<stdio.h>
#include<string.h>
int main()
{
	int a[32];
	int i=0,n,m,k;
	memset(a,0,32*sizeof(int));
	printf("请输入数字：");
	scanf("%d",&n);
	m=k=n;
	while(n!=0)
	{
		a[i++]=n%2;
		n=n/2;
	}
	printf("二进制：");
	for(i=31;i>=0;i--)
	{
		printf("%d",a[i]);
	}
	printf("\n");
	memset(a,0,32*sizeof(int));
	i=0;
	while(m!=0)
	{
		a[i++]=m%8;
		m=m/8;
	}
	printf("八进制：");
	for(i=31;i>=0;i--)
	{
		printf("%d",a[i]);
	}
	printf("\n");
	memset(a,0,32*sizeof(int));
	i=0;
	while(k!=0)
	{
		//if(k%16<10)
			//a[i++]=k%16;
		//else
			a[i++]=k%16;
		k=k/16;
	}
	printf("十六进制：");
	for(i=31;i>=0;i--)
	{
		if(a[i]>=10)
			printf("%x",a[i]);
		else 
			printf("%d",a[i]);
	}
	printf("\n");
}

/*int main()
{}*/

