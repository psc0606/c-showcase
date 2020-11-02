#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>



int main()
{
	pid_t pid;
	FILE* fp;
	fp = fopen("1.txt","r+");
	if(fp == NULL)
	{
		perror("open fd err");
		exit(1);
	}
	pid = fork();
	if(pid < 0)
	{
		perror("fork err.\n");
		exit(1);
	}
	else if(pid == 0)
	{
		char ch;
		while(1)
		{
			ch = fgetc(fp);
			if(ch == EOF)
				break;
			if(ch >= 'a' && ch <= 'z')
			{
				ch = ch - 32;
				fseek(fp,-1,SEEK_CUR);
				fputc(ch,fp);
			}
		}
		exit(0);
	}
	else
	{
		int ch;
		wait(NULL);
		rewind(fp);
		while(1)
		{
			ch = fgetc(fp);
			if(ch == EOF)
				break;
			if(ch == '\n')
				continue;
			if((ch <'A')|| (ch>'Z'&&ch<'a')|| (ch >'z'))
			{
				ch = ' ';
				fseek(fp,-1,SEEK_CUR);
				fputc(ch,fp);
			}
		}
	}
}
