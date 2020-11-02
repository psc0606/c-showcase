/*----------------------------------------
**	实现两个超大正整数的加法，至少10位
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>
void swap(char str[])
{
	int i, k;
	i = strlen(str);
	char c[20];
	strcpy(c,str);
	for (i--,k=0;i >= 0; i--,k++)
	{
		str[k]=c[i];
	}
}


void main()
{
	int i, j, k;
	char a[20], b[20];
	char c[20];
	scanf("%s",a);
	scanf("%s",b);
	j = strlen(a);
	k = strlen(b);
	swap(a);
	swap(b);
	printf("a的反序为 %s\n",a);
	printf("b的反序为 %s\n",b);
	if (j >= k)
	{
		for (k; k < j; k++)
		{
			b[k] = '0';
		}
		a[j] = '0';
		a[j+1] = '\0';
		b[k] = '0';
		b[k+1] = '\0';
	}
	if (j < k)
	{
		for (j; j <= k; j++)
		{
			a[j] = '0';
		}
		a[j] = '\0';
		b[k] = '0';
		b[k+1] = '\0';
	}
	
	printf("a的反序为 %s\n",a);
	printf("b的反序为 %s\n",b);
/******************************************
*******************相加*******************/	
	for (i = 0; a[i] != '\0'; i++)
	{
		if ((a[i] + b[i])%48 >= 10)
		{
			c[i] = (a[i] + b[i]) -58;
			a[i+1] = a[i+1] + 1;
		}
		else
		{
			c[i] = (a[i] + b[i])-48;
		}
		printf("%c",c[i]);
	}
	if (c[i-1] == 0)
	{
		c[i-1] == '\0';
	} 
	else 
	{
		c[i] = '\0';
	}
	printf("\n");
/******************************************
*******************打印*******************/	
	swap(c);
	printf("%s",c);
	printf("\n");
}
