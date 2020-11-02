#include<stdio.h>
#include"task1.h"
#include"task2.h"

int fun(char a,char b)
{
	if((a=='+' || a=='-') && (b=='*' || b=='/'))
	{
		return 1;
	}
	else return 0;
}

int fun1(int a,int b,char c)
{
		if(c=='+')
			return (a+b);
		if(c=='-')
			return (a-b);
		if(c=='*') 
			return (a*b);
		if(c=='/')
			return (int)(a/b);
}

int main()
{
	int a,b,m,i=0;
	char str[100],c[100];
	gets(str);
	char *p;
	for(p=str;*p!='\0';p++)
	{
		if(*p>='0' && *p<='9')
		{
			m=(int)(*p-'0');
			push1(m);
		}
		else
		{
			while(!is_empty2())
			{
				if(Top2()=='(')
				{
					push2(*p);
					break;			
				}
				if(*p=='(')
				{
					push2(*p);
					break;
				}
				else if(*p==')')
				{
					while(Top2()!='(')
					{
						if( fun(Top2(),*p))
						{
							push2(*p);
							break;
						}
						else
						{
							b=Top1();
							printf("b=%d\t",b);
							pop1();
							a=Top1();
							printf("a=%d\t",a);
							pop1();
							printf("(a,b)=%d\t",fun1(a,b,Top2()));
							push1(fun1(a,b,Top2()));
							pop2();
						}
					}
					pop2();
					break;
				}
				else if( fun(Top2(),*p)==1)
				{
					push2(*p);
					break;
				}
				else
				{
					b=Top1();
					printf("b=%d\t",b);
					pop1();
					a=Top1();
					printf("a=%d\t",a);
					pop1();
					printf("(a,b)=%d\t",fun1(a,b,Top2()));
					push1(fun1(a,b,Top2()));
					pop2();
				}
			}
			if(is_empty2() && *p!=')')
			{
				//printf("kkkkkk");
				push2(*p);
			}
		}
	}
	while(!is_empty2())
	{
		b=Top1();
		printf("b=%d\t",b);
		pop1();
		a=Top1();
		printf("a=%d\t",a);
		pop1();
		printf("(a,b)=%d\t",fun1(a,b,Top2()));
		push1(fun1(a,b,Top2()));
		pop2();
	}
}						


