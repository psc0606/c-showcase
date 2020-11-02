#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int new_exec(char* argv[])
{
	pid_t pid;
	pid = fork();
	
	if(pid < 0)
	{
		perror("fork err.");
		exit(1);
	}
	else if(pid == 0)
	{
		if(execvp(argv[0],argv) < 0)
		{
			perror("execvp err");
		}
		exit(3);
	}
	else
		wait(NULL);
	
}

int find(char* ptr)
{
	int i,n,len;
	n = 0;
	len = strlen(ptr);
	for(i=0;i<len;i++)
	{
		if(ptr[i] == '/')
		{
			n=i;
			continue;
		}
	}
	if(n == 0)
		return n;
	else
		return n+1;
}


char* do_cd(char* argv[])
{
	char* path = malloc(sizeof(char)*32);
	char pathname[256] = {0};
	int n;
	memset(path,0,sizeof(char)*32);
	if(argv[1] != NULL)
	{
		strcpy(pathname,argv[1]);
		if(chdir(argv[1]) < 0)
		{
			perror("chdir err");
			//return NULL
		}
		getcwd(pathname,sizeof(pathname));
		n = find(pathname);
		strcpy(path,&pathname[n]);
		return path;
	}
	return NULL;
}

int main()
{
	char* cmd = malloc(256*sizeof(char));
	char* cmd_arg[10];
	char* path = "user";
	int cmdlen,i,j,tag;
	if(chdir("/home/user")<0)
	{
		perror("chdir err");
		return -1;
	}
	do
	{
		memset(cmd,0,256);
		
		printf("-=Mini Shell %s=-$",path);
			
		fgets(cmd,256,stdin);
		cmdlen = strlen(cmd);
		cmdlen--;
		cmd[cmdlen] = '\0';
		i=0;j=0;tag =0;
		memset(cmd_arg,0,sizeof(cmd_arg));
		while(i<cmdlen && j <10)
		{
			if(cmd[i] == ' ')
			{
				cmd[i] = '\0';
				tag =0;
			}
			else
			{
				if(tag == 0)
					cmd_arg[j++] = cmd+i;
				tag =1;
			}
			i++;
		}
		if(j>=10 && i< cmdlen)
		{
			printf("Too Many Arguments");
			continue;
		}
		/// quit command
		if(strcmp(cmd_arg[0],"quit") == 0)
			break;
		if(strcmp(cmd_arg[0],"cd") == 0)
		{
			path = do_cd(cmd_arg);
			continue;
		}
		// new execute
		new_exec(cmd_arg);
		
	}while(1);
	
}
