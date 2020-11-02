/*----------------------------------------
**	3. 从终端输入n个人的姓名，电话，组成一个
**	通讯录，对此通讯录进行排序，输出排序后的结果
**	（要求用直接插入排序法）。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void main()
{
	int i, j, n;
	printf("input n = ");
	scanf("%d",&n);
	char a[n][10], b[n][12], k[10], m[12];
	for (i = 0; i < n; i++)
	{
		printf("input name:\n");
		scanf("%s",a[i]);
		printf("input telephone numbers:\n");
		scanf("%s",b[i]);
	}
	
	for (i = 1; i < n; i++)
	{
		strcpy(k,a[i]);
		strcpy(m,b[i]);
		j = i - 1;
		while (j >= 0 && strcmp(k,a[j]) < 0)
		{
			strcpy(a[i],a[j]);
			strcpy(b[i--],b[j--]);
		}
		strcpy(a[i],k);
		strcpy(b[i],m);
	}
	
	printf("The name is \n");
	for (i = 0; i < n; i++)
	{
		printf("%s,tele %s\n",a[i],b[i]);
	}
}
