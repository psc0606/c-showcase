/*----------------------------------------
**	3. 编写一个名为sort的函数，它用于对任何
**	类型数组进行排列。为了使函数更为通用，它
**	的其中一个参数必须是一个指向比较回调函数
**	的指针，该回调函数由调用程序提供。比较函
**	数接受两个参数，也就是两个指向需要进行比
**	较的值的指针。如果两个值相等，函数返回0，
**	如果第一个小于第二个，函数返回一个小于0
**	的整数，如果第一个大于第二个，函数返回一
**	个大于0的整数。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

int fun(void *a, void *b)
{
	int *x = (int *)a;
	int *y = (int *)b;
	
	if (*x > *y)
	{
		return 1;
	}
	if (*x == *y)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int fun1(void *a, void *b)
{
	float *x = (float *)a;
	float *y = (float *)b;
	
	if (*x > *y)
	{
		return 1;
	}
	if (*x == *y)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int fun2(void *a, void *b)
{
	char *x = (char *)a;
	char *y = (char *)b;
	
	if (*x > *y)
	{
		return 1;
	}
	if (*x == *y)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int sort(void *a,size_t len,size_t size,int (*p)(void *,void *))
{
	char *ptr = (char *)a;
	int i, j, k;
	char *q = malloc(size);
	
	for (i = 0; i < len-1; i++)
	{
		for (j = 0; j < len-i-1; j++)
		{
			if (p(ptr+j*size,ptr+(j+1)*size) > 0)		//p的应用！
			{
				memcpy(q,ptr+j*size,size);
				memcpy(ptr+j*size,ptr+(j+1)*size,size);
				memcpy(ptr+(j+1)*size,q,size);
			}
		}
	}
	
	free(q);
}


void main()
{
	char a[10]={"asdf"};
	int b[10]={5,6,4,3};
	int i,c = 4, d = 6;
//	printf("input string:\n");
//	scanf("%s",a);
//	scanf("%s",b);
	sort(b,4,sizeof(int),fun);
	sort(a,4,sizeof(char),fun2);
	printf("%3s\n",a);
	for (i = 0; i < 4; i++)
	{
		printf("%3d\n",b[i]);
	}
}
