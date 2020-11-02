#include <stdio.h>

int port(int a[], int l, int r)
{
	int p = a[l];
	while (l < r)
	{
		while (l < r && p <= a[r])
		{
			--r;
		}
		a[l] = a[r];
		while (l < r && p >= a[l])
		{
			++l;
		}
		a[r] = a[l];
	}
	a[l] = p;
	return l;
}

void sort(int a[], int l, int r)
{
	int q = port(a,l,r);
	if (l < r)
	{
		sort(a,l,q-1);
		sort(a,q+1,r);
	}
}

void main()
{
	int i,n;
	printf("input n = ");
	scanf("%d",&n);
	int a[n];
	getchar();
	for (i = 0; i < n; i++)
	{
		scanf("%d",&a[i]);
	}
	sort(a,0,n-1);
	for (i = 0; i < n; i++)
	{
		printf("%3d",a[i]);
	}
}
