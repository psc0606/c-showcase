/*----------------------------------------
**	3. 随便输入一个整数，转换成字符串输出。
**----------------------------------------
*/

#include <stdio.h>

void main()
{
	long long n;
	int i = 0, j, a[20];
	char b[20];
	printf("input number:");
	scanf("%lld",&n);
	while (n % 10)
	{
		a[i++] = (n % 10);
		n = n / 10;
	}

	for (i--,j = 0; i >= 0; i--,j++)
	{
		b[j] = a[i] + '0';
	}
	b[j] = '\0';
	printf("%s\n",b);
}
