#include<stdio.h>
#include<string.h>
int main()
{
	char a[]="aaafceeddddcwwde",b[18];
	memset(b,'\0',18);
	char *p1,*p2;
	int count=0;
	p1=a;
	p2=b;
	while(*p1!='\0')
	{
		*p2=*p1;
		while(1)
		{
			if(*(++p1)==*p2)
				count++;
			else
			{
				break;
			}
		}
		if(count!=0)
		{
			*(++p2)=count+1+'0';
			count=0;
		}
		p2++;
	}
	printf("%s",b);
}
