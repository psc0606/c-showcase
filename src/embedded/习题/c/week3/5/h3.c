/*----------------------------------------
**	3. 从键盘输入10个用户的数据，包括：姓名
**	和电话号码。要求按姓名排序后，输出用户的
**	全部数据。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
#define x 3

struct
{
	char name[10];
	char tele[15];
}n[x];

void main()
{
	int i, j;
	char a[10],b[15];
	for (i = 0; i < x; i++)
	{
		printf("输入第%d个用户的姓名:\n",i+1);
		scanf("%s",n[i].name);
		printf("输入第%d个用户的电话号码:\n",i+1);
		scanf("%s",&n[i].tele);
	}
	
	for (i = 1; i < x; i++)
	{
		j = i - 1;
		strcpy(a,n[i].name);
		strcpy(b,n[i].tele);
		while (j >= 0 && strcmp(a,n[j].name) < 0)
		{
			strcpy(n[i].name,n[j].name);
			strcpy(n[i--].tele,n[j--].tele);
		}
		strcpy(n[i].name,a);
		strcpy(n[i].tele,b);
	}
	
	for (i = 0; i < x; i++)
	{
		printf("%s%15s\n",n[i].name,n[i].tele);
	}
}
