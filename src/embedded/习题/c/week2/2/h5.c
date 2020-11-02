/*----------------------------------------
**	5. 分别实现编程strlen,strcmp,strcpy
**	,memmove的操作。
**----------------------------------------
*/
/*
#include <stdio.h>
#include <string.h>
void main()
{
	char a[20], b[20];
	printf("input string a\n");
	scanf("%s",a);
	printf("input string b\n");
	scanf("%s",b);
	printf("strlen a = %d\n",strlen(a));
	printf("strlen b = %d\n",strlen(b));
	printf("strcmp (a,b) = %d\n",strcmp(a,b));
	printf("strcpy (a,b) = %s\n",strcpy(a,b));
	memmove(&a[2],&b[3],4*sizeof(char));
	printf("memmove (a[2],b[3],4) = %s\n",a);
}
*/

#include <stdio.h>
int len(char *p)
{
	int i = 0, j = 0;
	while (*(p+i) != '\0')
	{
		i++;
		j++;
	}
	return j;
}

int cmp(char *p,char *q)
{
	int i = 0;
	while (1)
	{
		i++;
		if (*(q+i)=='\0'&&*(p+i)=='\0')
		{
			return 0;
		}
		if( *(p+i)>*(q+i)|| *(q+i)=='\0')
		{
			return 1;
		}
		if ( *(p+i)<*(q+i)|| *(p+i)=='\0')
		{
			return -1;
		}
	}
}

char *cpy(char *p,char *q)
{
	int i, j;
	for (i = 0; *(q+i) != '\0'; i++)
	{
		*(p + i) = * (q + i);
	}
	*(p + i) = '\0';//很重要！！！！！！！
	return p;
}

void move(void *dest, const void *src, size_t n)
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
//	return dest;
}

void main()
{
	char a[20], b[20];
	printf("input string a\n");
	scanf("%s",a);
	printf("input string b\n");
	scanf("%s",b);
	printf("len a = %d\n",len(a));
	printf("len b = %d\n",len(b));
	printf("cmp (a,b) = %d\n",cmp(a,b));
	printf("cpy (a,b) = %s\n",cpy(a,b));
	printf("b = %s\n",b);
	move (&a[2],&b[3],4*sizeof(char));
	printf("move (a[2],b[3],4) = %s\n",a);
}

