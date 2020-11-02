//5. 分别实现编程strlen,strcmp,strcpy,memmove的操作。

#include<stdio.h>

int length(char *string)
{
	int i=0;
	while(*(string+i)!='\0')
		i++;
	return i;
}
int compare(char *str1,char *str2)
{
	int i=0,j=0,l;
	while(*(str1+i)!='\0')
		i++;
	while(*(str2+j)!='\0')
		j++;
	if(i>j)
		l=i;
	else  
		l=j;
	for(i=0;i<l;i++)
	{
		if(*(str1+i)>*(str2+i))
		{
			return 2;
			break;
		}
		else if(*(str1+i)<*(str2+i))
		{
			return 1; 
			break;
		}
	}
	if(i==l)
		return 0;	
}
char *copy(char *str,char *string)
{
	int i;
	for(i=0;*(string+i)!='\0';i++)
	{
		*(str+i)=*(string+i);
	}
	*(str+i)='\0';
	return(str);
}


void *move(void *str,void *string,size_t n )
{
	char *str1=(char*)str;
	char *str2=(char*)string;
	int i;
	if(str1>str2 && str1<str2+n)
	{
		for(i=n-1;i>=0;i--)
		{
			*(str1+i)=*(str2+i);
			//printf("%c\n",*(str1+i));
			//printf("%d\n",i);
		}
	}
	else
	{
		for(i=0;i<n;i++)
		{
			*(str1+i)=*(str2+i);
			//printf("%c\n",*(str1+i));
		}
	}
	return(str1);
}

int main()
{
	char s1[100],s2[100];
	int i,j,m;
	size_t n;
	printf("请输入s1:");
	gets(s1);
	printf("请输入s2:");
	gets(s2);
	//printf("strlen(s1)=%4d\n",length(s1));
	i=compare(s1,s2);
	printf("strcmp(s1,s2)的结果为:");
	if(i==2)
		printf("s1大\n");
	 if(i==1)
		printf("s2大\n");
	 if(i==0)
		printf("相等\n");
	/* copy(s2,s1);
	 printf("copy(s2,s1)=:");
	 puts(s2);*/
	/*printf("请输入复制的字节数n:");
	scanf("%d",&n);
	printf("请输入开始复制出的位置s1[m]:");
	scanf("%d",&m);
	printf("请输入开始复制进的位置s1[j]:");
	scanf("%d",&j);
	move(s1+j,s2+m,n);
	printf("menmove(s1[%d],s2[%d],%d):",j,m,n);
	puts(s1);	*/
}
