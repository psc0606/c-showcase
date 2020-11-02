#include<stdio.h>
#include<string.h>
int fun1(char x)
{
	int a;
	a=x-48;
	return a;	
}

char fun2(int x)
{
	char a;
	a=x+48;
	return a;
}

void nixu(char x[100])
{
	char y[100];
	strcpy(y,x);
	int i,j=strlen(x)-1;
	for(i=0;i<strlen(x);i++)
	{
		x[j]=y[i];
		j--;
	}
}
int main()
{
	int i,m,n=0;
	char a[100],b[100],c[100];
	memset(a,'\0',100);
	memset(b,'\0',100);
	memset(c,'\0',100);
	gets(a);
	gets(b);
	nixu(a);
	printf("a的逆序:%s\n",a);
	nixu(b);
	printf("b的逆序:%s\n",b);
	memset(((strlen(a)>strlen(b))?b:a)+((strlen(a)>strlen(b))?strlen(b):strlen(a)),48,abs(strlen(a)-strlen(b)));
	for(i=0;i<strlen(a);i++)
	{
		m=fun1(a[i])+fun1(b[i]);
		if(m>=10)
		{
			c[i]=fun2(m%10+n);
			n=m/10;
		}
		else
		{
			if(m+n>=10)
			{
				c[i]=fun2((m+n)%10);
				n=(m+n)/10;
			}
			else
			{
				c[i]=fun2(m+n);
				n=0;
			}
		}
	}
	if(n==1)
	{
		c[i]=49;
	}
	nixu(c);
	printf("结果=%s",c);
}
