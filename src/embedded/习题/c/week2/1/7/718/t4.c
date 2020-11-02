//4.实现两个超大正整数的加法，至少10位。
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
	memset(c,'\0',100);
	printf("input number:");
	gets(a);
	printf("input number:");
	gets(b);
	nixu(a);
	printf("a的逆序:%s\n",a);
	nixu(b);
	printf("b的逆序:%s\n",b);
	for(i=0;i<((strlen(a)<strlen(b))?strlen(b):strlen(a));i++)
	{
		if(i<((strlen(a)>strlen(b))?strlen(b):strlen(a)))
		{
			m=fun1(a[i])+fun1(b[i]);
			if(m>=10)
			{
				//printf("n=%d\n",n);
				//printf("num=%d",m%10+n);
				c[i]=fun2(m%10+n);
				n=m/10;
			}
			else
			{
				//printf("n=%d\n",n);
				//printf("num=%d",m+n);
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
		else
		{
			if(n==1)
			{
				c[i]=((strlen(a)>strlen(b))?a[i]:b[i])+1;
				if(c[i]==58)
				{
					c[i]=48;
					n=1;
				}
				n=0;
			}	
			else
				c[i]=((strlen(a)>strlen(b))?a[i]:b[i]);
		}
	}
	//printf("c=%s\n",c);
	//printf("i=%d\n",i);
	nixu(c);
	printf("result=%s\n",c);
}
