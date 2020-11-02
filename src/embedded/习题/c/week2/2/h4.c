/*----------------------------------------
**	4. 编写一个函数，从一个字符串中去除多余
**	的空格。函数原型如下：
**	void deblank(char* string)
**	当函数发现字符串中如果有一个地方由多个连续
**	的空格组成，就把它们改成单个空格字符.
**----------------------------------------
*/

#include <stdio.h>
void deblank(char *string)
{
	int i, j, k;
	for (i = 0; *(string+i) != '\0'; i++)
	{
		if (*(string+i) == 32)
		{
			if (*(string+i+1) != 32)
			{
//				for (j = i; *(string+j) != '\0'; j++)
//				{
//					*(string+j) = *(string+j+1);
//				}
				continue;
			}
			else
			{
				for (j = i;*(string+j) == 32;j++)
				{
				}
				for (j--,k=i; *(string+k) != '\0'; j++,k++)
				{
					*(string+k) = *(string+j);
				}
			}
		}
	}
}

void main()
{
	char a[20], *p;
	printf("input string:\n");
	gets(a);
	p = a;
	deblank(p);
	printf("%s",a);
}
