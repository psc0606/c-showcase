/*----------------------------------------
**	5. 编码完成下面的处理函数。函数将字符串中
**	的字符'*'移到串的前部分，前面的非'*'字符
**	后移，但不能改变非'*'字符的先后顺序，函数
**	返回串中字符'*'的数量。如原始串为：
**	ab**cd**e*12，处理后为*****abcde12，
**	函数并返回值为5。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
/*
int sort(char *a)
{
	int i, n = 0;
	for (i = 0; *(a+i) != '\0'; i++)
	{
		if (*(a+i) == '*')
		{
			if (i != n)
			{
				memmove(a+n+1,a+n,(i-n)*sizeof(char));
				*(a+n) = '*';
			}
			n++;
		}
	}
	return n;
}
*/
int sort(char *a)
{
	int i, j, n = 0;
	char b;
	for (i = 0; *(a+i) != '\0'; i++)
	{
		if (*(a+i) == '*')
		{
			for (j = i-1; j >= 0 && *(a+j) != '*'; j--)
			{
				*(a+j+1) = *(a+j); 
			}
			*(a+j+1) = '*';
			n++;
		}
	}
	return n;
}

void main()
{
	int i, n;
	char a[20];
	scanf("%s",a);
	n = sort(a);
	printf("%s\n",a);
	printf("num = %d\n",n);
}
