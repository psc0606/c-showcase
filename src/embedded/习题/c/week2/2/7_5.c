/*5. 分别实现编程strlen,strcmp,strcpy,memmove的操作*/
#include<stdio.h>

unsigned int strlen(const char *p)
{
	int i, length = 0;
	for (i = 0; *(p + i) != '\0'; i++)
	{
		length++;
	}
	return length;
}

int strcmp(const char *p1, const char *p2)
{
	int i=0;
	while(*(p1+i) != '\0' || *(p2+i) != '\0')
	{	
		if(*(p1+i) > *(p2+i))
		{
			return 1;
		}
		else if(*(p1+i) < *(p2+i))
			   {
			   		return -1;
			   }
			   else i++;
	}
	return 0;
}

char *strcpy(char *dest, const char *src)
{	
	int i;
	for (i = 0; *(src + i) != '\0'; i++)
	{
		*(dest + i) = *(src + i);
	}
	*(dest + i) = '\0';
	return dest;
}


void *memmove(void *dest, const void *src, size_t n)
{
	size_t i;
	char *d = (char *)dest;
	char *s = (char *)src;
	if(d > s && d < s + n)
	{
		while(n--)
		{
			*(d + n) = *(s + n);
		}
	/*	for(i = n-1; i >= 0; i--)    //也可以用这种方法
		{
			*(d + i) = *(s + i + (s-d));
		}   */
	}
	else 
	{
		while(n--)
		{
			*d++ = *s++;
		}
	} 
	return dest;
}

void main()
{
	int n1, n2, n3, n4;
	char str[100], str1[100], str2[100];
	
	n1 = strlen(str);
	
	gets(str1);
	gets(str2);
//	n2 = strcmp(str1,str2);
//	strcpy(str1,str2);
	printf("请输入复制的字节数:\n");
 	scanf("%d",&n4);
   memmove(str1+3,str1,n4);
	puts(str1);
	
}
