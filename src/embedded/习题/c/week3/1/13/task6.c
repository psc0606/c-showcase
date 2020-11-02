#include<stdio.h>
#include<string.h>

/*void check(char *a,char *b,int(*cmp)(const char*,const char *));

void main()
{
	char s1[80],s2[80];
	int (*p)(const char*,const char*);
	p=strcmp;
	
	printf("two strings:");
	gets(s1);
	gets(s2);
	check(s1,s2,p);
}

void check(char *a,char *b,int(*cmp)(const char*,const char *))
{
	if(!(*cmp)(a,b))
		printf("equal");
	else
		printf("no equal");
}*/

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

double add(double x,double y)
{
	return x+y;
}

double sub(double x,double y)
{
	return x-y;
}

double mul(double x,double y)
{
	return x*y;
}

double div(double x,double y)
{
	if(y==0)
		printf("err");
	return x/y;
}

int main()
{
	double (*oper_func[])(double,double)={add,sub,mul,div};
	int oper;
	scanf("%d",&oper);
	double result = oper_func[oper](1.5,3.6);
	printf("result:%lf\n",result);
}
