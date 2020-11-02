/*----------------------------------------
**	1． 使用fread和fwrite实现文件的拷贝，相当
**	于cp命令的用能。
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	FILE *fp1, *fp2;
	long int size;
	char buf[1024];
	if (argc != 3)
	{
		perror("format err");
		exit(0);
	}
	if ((fp1 = fopen(argv[1],"r+")) == NULL)
	{
		perror("open err");
		exit(0);
	}
	if ((fp2 = fopen(argv[2],"w+")) == NULL)
	{
		perror("open err");
		exit(0);
	}
	
	fseek(fp1,0,SEEK_END);
	size = ftell(fp1);
	rewind(fp1);
	if (fread(buf,sizeof(char),size,fp1) < 0)
	{
		perror("error");
		exit(0);
	}
	fwrite(buf,sizeof(char),size,fp2);
	
	fclose(fp1);
	fclose(fp2);
	return 0;
}
