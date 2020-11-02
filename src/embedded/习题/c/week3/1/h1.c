/*----------------------------------------
**	1.输入一段英文，从中找出最长的单词。
**----------------------------------------
*/

#include <stdio.h>
#include <string.h>

void check(char *a)
{
	int i, j, k = 0, max, b[20];
	char d[100][100];
/*	for (i = 0; i < len; i++)
	{
		j = i;
		while (*(a+j) != ' '&&*(a+j) != '\0')
		{
			j++;	
		}
		b[k] = j - i;
		memcpy(d[k],a+i,(j-i)*sizeof(char));
		d[k++][j-i]='\0';
		i = j;
	}
*/	
	for (i = 0; *(a+i) != '\n'; i++)
	{
		j = i;
		while (*(a+j) != ' '&&*(a+j) != '\n')
		{
			if((*(a+j) >= 'A'&&*(a+j) <= 'Z')||(*(a+j) >= 'a'&&*(a+j) <= 'z'))
			{
				j++;
			}
			else
			{
				break;	
			}
		}
		b[k] = j - i;
		memcpy(d[k],a+i,(j-i)*sizeof(char));
		d[k++][j-i]='\0';
		if (*(a+j) == '\n')
		{
			break;
		}
		i = j;
	}
	max = b[0];
	for (i = 0,j = 0; i < k; i++)
	{
		if (max < b[i])
		{
			j = i;
			max = b[i];
		}
	}
	printf("The word is");
	for (i = 0; i < 20; i++)
	{
		if (b[j] == b[i])
		printf(" %s ",d[i]);
	}
	printf("\n");
}

void main()
{
	char a[100];
	printf("input sentences:\n");
	fgets(a,100,stdin);
	check(a);
}
