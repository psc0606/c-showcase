/*----------------------------------------
**	2． 已知一个文本文件，写程序实现对该文件的操作：
**	a. 查找相应单词的个数
**	b. 把原来单词替换成新单词
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int judge_a(char b)
{
	if ((b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z'))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int judge(char *buf,char *b,int i,int len)
{
	int j;
	if ((i == 0) || !judge_a(*(buf+i-1)))
	{
		for (j = 0; *(buf+i+j) != '\n';)
		{
			if (judge_a(*(buf+i+j)) && (*(buf+i+j) == *(b+j)))
			{
				j++;
			}
			else
			{
				break;
			}
		}
		if (j == len)
		{
			if (!judge_a(*(buf+i+j)))
			{
				return 1;
			}
		}
	}
	return 0;
}

int main(int argc,char **argv)
{
	FILE *fp, *fp1;
	int i, num = 0, len1, len2;
	char buf[1024], *a;
	len1 = strlen(argv[3]);
	len2 = strlen(argv[4]);
	if (argc != 5)
	{
		perror("error");
		exit(0);
	}
	if ((fp = fopen(argv[1],"r")) == NULL)
	{
		perror("error");
		exit(0);
	}
	if ((fp1 = fopen(argv[2],"w")) == NULL)
	{
		perror("error");
		exit(0);
	}
	
	while (1)
	{
		a = fgets(buf,sizeof(buf),fp);
		if (feof(fp))
		{
			break;
		}
		
		for (i = 0; *(buf+i) != '\n'; i++)
		{
			if ((*(buf+i) == argv[3][0]) && (judge(buf,argv[3],i,len1)))
			{
				num++;
				if (len1 != len2)
				{
					memmove(buf+i+len2,buf+i+len1,strlen(buf)*sizeof(char));
				}
				memmove(buf+i,argv[4],len2*sizeof(char));
				i += len2;
			}
		}
		fputs(buf,fp1);
	}
	
	printf("the nmuber is %d\n",num);
	fclose(fp);
	fclose(fp1);
	return 0;
}
