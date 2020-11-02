#include<stdio.h>
#include<string.h>
#include<math.h>
int fun(int n)
{
	int m=0;
	while(n!=0)
	{
		m++;
		n=n/10;
	}
	return m;
}


int main()
{
	int n,i,k;
	char a[100];
	memset(a,'\0',100);
	printf("please input n:");
	scanf("%d",&n);
	k=n;
	//printf("%d",fun(n));
	for(i=0;i<fun(n);i++)
	{
		//printf("k=%d\n",k);
		a[i]=(char)(k/((int)pow(10,fun(k)-1))+48);
		k=k%((int)pow(10,fun(k)-1));
	}
	a[i]='\0';
	printf("%s",a);
}
