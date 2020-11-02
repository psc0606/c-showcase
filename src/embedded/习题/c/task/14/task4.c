#include<stdio.h>
#include<string.h>

char fun(int x)
{
	char n=(char)(x+48);
	return n;
}

int main()
{
	char str[100],*p,*q,str1[100];
	memset(str,'\0',100);
	memset(str1,'\0',100);
	gets(str);
	int i=0,n=1,flag=0;
	p=str;
	str1[i]=*p;
	for(q=p+1;*q!='\0';q++)
	{
		if(*q==*p)
		{
			n++;
			//printf("n=%d\n",n);
			flag=1;
		}
		else if(flag==1)
		{
			i++;		
			str1[i++]=fun(n);
			flag=0;
			n=1;
			str1[i]=*q;
			p=q;
		}
	}
	str1[i+1]=fun(n);
	printf("%s",str1);
}





