/*----------------------------------------
**	 1、 Sn=a+aa+aaa+......+aaa...aa的值
**	其中a表示一个数字,n表示a的位数
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i, a, n, s, sum = 0;
	printf("请输入a和n的值:");
	scanf("%d%d",&a,&n);
	for (i = 1, s = a; i < n; i++)
	{
		sum = sum + s;
		s = a +s * 10;
		printf("%d\n",s);
	}
	printf("%d\n",sum+s);
}
