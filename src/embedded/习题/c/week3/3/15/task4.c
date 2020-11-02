#include<stdio.h>

int quicksort_division(int *p,int left,int right)
{
	int base=p[left];
	while(left<right)
	{
		while(left<right && p[right]>=base)
		{
			--right;
		}
		p[left]=p[right];
		while(left<right && p[left]<=base)
		{
			++left;
		}
		p[right]=p[left];
	}
	p[left]=base;
	return left;
}

void quicksort(int *p,int left,int right)
{
	int n_base;
	if(left<right)
	{
		n_base=quicksort_division(p,left,right);
		quicksort(p,left,n_base-1);
		quicksort(p,n_base+1,right);
	}
}

int main()
{
	int a[100],n,i;
	printf("please input n:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	quicksort(a,0,n-1);
	printf("the sorted numbers:\n");
	for(i=0;i<n;i++)
	{
		printf("%d\t",a[i]);
	}
}



