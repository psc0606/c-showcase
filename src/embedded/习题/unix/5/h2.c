/*----------------------------------------
**	2.使用标准IO实现文件的拷贝，并统计文本文件中的行数。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE *fp1, *fp2;
	int num = 0;
	char buf[1024];
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if ((fp1 = fopen(argv[1],"r")) == NULL)
	{
		perror("open 1 err");
		exit(0);
	}
	if ((fp2 = fopen(argv[2],"w+")) == NULL)
	{
		perror("open 2 err");
		exit(0);
	}
	while (1)
	{
		fgets(buf,sizeof(buf),fp1);
		if (feof(fp1))
		{
			break;
		}
		fputs(buf,fp2);
		num++;
	}
	printf("num = %d\n", num);
}
