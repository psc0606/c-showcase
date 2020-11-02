//3. 编写一个名为sort的函数，它用于对任何类型数组进行排列。为了使函数更为通用，它的其中一个参数必须是一个指向比较回调函数的指针，该回调函数由调用程序提供。比较函数接受两个参数，也就是两个指向需要进行比较的值的指针。如果两个值相等，函数返回0，如果第一个小于第二个，函数返回一个小于0的整数，如果第一个大于第二个，函数返回一个大于0的整数
#include<stdio.h>
#include<string.h>
#include<malloc.h>

int sort(void* ptr,size_t len,size_t size,int (*p)(void*,void*))
{
	char* ptr1 = (char*)ptr;
	int i,j,k;
	void* temp = malloc(size);
	
	for(i=0;i<len-1;i++)
	{
		for(j=0;j<len-i-1;j++)
		{
			if(p(ptr1+j*size,ptr1+(j+1)*size)> 0)
			{
				memcpy(temp,ptr1+j*size,size);
				memcpy(ptr1+j*size,ptr1+(j+1)*size,size);
				memcpy(ptr1+(j+1)*size,temp,size);
			}
		}
	}
	free(temp);
}
int float_cmp(void* x,void* y)
{
	float* ptr1 = (float*)x;
	float* ptr2 = (float*)y;
	if(*ptr1 >*ptr2)
		return 1;
	else if(*ptr1 == *ptr2)
		return 0;
	else
		return -1;
}


/*int int_cmp(void* x,void* y)
{
	int* ptr1 = (int*)x;
	int* ptr2 = (int*)y;
	if(*ptr1 >*ptr2)
		return 1;
	else if(*ptr1 == *ptr2)
		return 0;
	else
		return -1;
}
*/

int main()
{
	float a[10];
	int i;
	for(i=0;i<10;i++)
		scanf("%f",a+i);
	sort(a,10,sizeof(float),float_cmp);
	for(i=0;i<10;i++)
		printf("%3.3f ",a[i]);
	printf("\n");
}
	
