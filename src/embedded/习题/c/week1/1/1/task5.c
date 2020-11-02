#include<stdio.h>
int main()
{
	int a[100];
	int n;
	int i;
	char str[]="open",str1[100];
	printf("how many numbers do you want enter:");
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	printf("密码：");
	
	while(1)
	{
		scanf("%s",str1);
		if(strcmp(str,str1)==0)
		{
			for(i=0;i<n;i++)
			{
				printf("%d",a[i]);
			}
			break;
		}
		else
		{
			printf("请重新输入：");
		}
	}
}
