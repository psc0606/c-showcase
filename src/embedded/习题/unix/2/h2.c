/*----------------------------------------
**	2． 建立一个txt文本文件，存放姓名和电话，相
**	当于一个通讯录。写一个程序，实现：
**	a． 对通讯录的插入功能
**	b． 通过姓名查找电话号码
**	c． 输入姓名删除相应记录
**	d． 查看通讯录的全部内容
**	e． 对通讯录的全部信息按照姓名排序
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct
{
	char name[10];
	char tel[15];
}Node;

Node n, m;

char buf[25];

void insert(FILE *fp)
{
	printf("input name and tel:\n");
	getchar();
	fgets(buf,sizeof(buf),stdin);
	if (sscanf(buf,"%s%s", n.name, n.tel) < 2)
	{
		printf("input err\n");
		return;
	}
	fseek(fp,0,SEEK_END);
	fputs(buf,fp);
}

void seek(FILE *fp)
{
	int i = 0;
	printf("input the seek name:\n");
	getchar();
	scanf("%s", n.name);
	rewind(fp);
	while (1)
	{
		if (fgets(buf,sizeof(buf),fp) == NULL)
		{
			break;
		}
		sscanf(buf, "%s", m.name);
		if (strcmp(m.name, n.name) == 0)
		{
			printf("%s",buf);
			i = 1;
		}
	}
	
	if (i == 0)
	{
		printf("no find\n");
	}
	getchar();
}

void delete(FILE *fp,char *argv[1])
{
	int i = 0, l = 0;
	char **array;
	array = (char **)malloc(25*sizeof(char *));
	printf("input the delete name:\n");
	getchar();
	scanf("%s", n.name);
	rewind(fp);
	while (1)
	{
		if (fgets(buf,sizeof(buf),fp) == NULL)
		{
			break;
		}
		sscanf(buf, "%s", m.name);
		if (strcmp(m.name, n.name) == 0)
		{
			i++;
			continue;
		}
		array[l] = (char *)malloc(25*sizeof(char));
		strcpy(array[l++],buf);
	}
	
	if (i == 0)
	{
		printf("no find\n");
	}
	fclose(fp);
	fp = fopen(argv[1],"w+");
	i = 0;
	while (1)
	{
		if (i == l)
		{
			break;
		}
		fputs(array[i], fp);
		free(array[i++]);
	}
	free(array);
	getchar();
}

void sort(FILE *fp)
{
	int l = 0, i, j, k;
	char **array;
	array = (char **)malloc(25*sizeof(char *));
	rewind(fp);
	while (1)
	{
		if (fgets(buf,sizeof(buf),fp) == NULL)
		{
			break;
		}
		array[l] = (char *)malloc(25*sizeof(char));
		strcpy(array[l++],buf);
	}
	
	for (i = 0; i < l-1; i++)
	{
		k = i;
		for (j = i+1; j < l; j++)
		{
			if (strcmp(array[k],array[j]) > 0)
			{
				k = j;
			}
		}
		if (k != i)
		{
			strcpy(buf,array[i]);
			strcpy(array[i],array[k]);
			strcpy(array[k],buf);
		}
	}
	
	rewind(fp);
	i = 0;
	while (1)
	{
		if (i == l)
		{
			break;
		}
		fputs(array[i], fp);
		free(array[i++]);
	}
	free(array);
	getchar();
}

void display(FILE *fp)
{
	rewind(fp);
	while (1)
	{
		if (fgets(buf, sizeof(buf), fp) == NULL)
		{
			break;
		}
		printf("%s",buf);
	}
	getchar();
}

int main(int argc, char **argv)
{
	char ch;
	FILE *fp, *fp1;
	
	if (argc != 2)
	{
		perror("format err\n");
		exit(0);
	}
	if ((fp = fopen(argv[1],"r+")) == NULL)
	{
		perror("open error");
		exit(0);
	}
	
	printf("\n*******************************\n");
	printf("a． 对通讯录的插入功能\n");
	printf("b． 通过姓名查找电话号码\n");
	printf("c． 输入姓名删除相应记录\n");
	printf("d． 查看通讯录的全部内容\n");
	printf("e． 对通讯录的全部信息按照姓名排序\n");
	printf("f． 退出\n");
	printf("*******************************\n\n");
	
	while (1)
	{
		printf("please input choose   ");
		switch (ch = getchar())
		{
			case 'a':
				insert(fp);
				break;
			case 'b':
				seek(fp);
				break;
			case 'c':
				delete(fp,argv);
				break;
			case 'd':
				display(fp);
				break;
			case 'e':
				sort(fp);
				break;
			case 'f':
				exit(0);
			default:
				getchar();
				break;
		}
	}
}
