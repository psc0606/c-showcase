/*----------------------------------------
**	1.通过main()函数的参数给定一个文本文件名及
**	其路径，统计其中的单词个数、大小写个数、标点
**	符号个数，行数。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argv,char **argc)
{
	FILE *fp;
	int a, space = 0, WORD = 0, word = 0, symbol = 0,line = 0;
	char b[1024];
	if (argv != 2)
	{
		perror("error");
		exit(0);
	}
	if ((fp = fopen(argc[1],"r")) == NULL)
	{
		perror("error");
		exit(0);
	}
	while (1)
	{
		a = fgetc(fp);
		if (feof(fp))
		{
			break;
		}
		else if (a == 32)
		{
			space++;
		}
		else if (a > 32 && a < 48)
		{
			symbol++;
		}
		else if (a >= 97 && a <= 122)
		{
			word++;
		}
		else if (a >= 65 && a <= 90)
		{
			WORD++;
		}
		else if (a == '\n')
		{
			line++;
		}
	}
	printf("the word is %d\n",space+1);
	printf("the symbol is %d\n",symbol);
	printf("the A word is %d\n",WORD);
	printf("the a word is %d\n",word);
	printf("the line is %d\n",line);
	fclose(fp);
	return 0;
}
