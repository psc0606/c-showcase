/*----------------------------------------
**	4. 从键盘输入n个整数，要求对输入的数字
**	进行从小到大排序，然后输出排序后的结果
**	（要求用快速排顺序法实现）。
**----------------------------------------
*/

#include <stdio.h>

int partitions(int a[],int l, int r)
{
	int p = a[l];	//a[0] = a[l];
	while (l < r)
	{
		while (l < r && a[r] >= p)
		{
			--r;
		}
		a[l] = a[r];
		while (l < r && a[l] <= p)
		{
			++l;
		}
		a[r] = a[l];
	}
	//a[l] = a[0];
	a[l] = p;
	return l;
}

void sort(int a[], int l, int r)
{
	int q;
	if (l < r)
	{
		q = partitions(a,l,r);
		sort(a,l,q-1);
		sort(a,q+1,r);
	}
}

void main()
{
	int i, j, k, n;
	printf("input n = ");
	scanf("%d",&n);
	getchar();
	int a[n];
	
	for (i = 0; i < n; i++)
	{
		scanf("%d",&a[i]);
	}
	
	sort(a,0,n);
	
	for (i = 0; i < n; i++)
	{
		printf("%3d",a[i]);
	}
	printf("\n");
}
