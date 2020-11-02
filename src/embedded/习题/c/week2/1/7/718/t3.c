/*2.输出“魔方阵”。所谓魔方阵是指这样的方阵，它的每一行，每一列和对角线之和均相等。例如，三阶魔方阵为：
  	 8	1	6
	3	5	7
	4	9	2*/
#include<stdio.h>
#include<stdlib.h>
int main()
{
	int **array;
	int step,i,j,k=1,flag=0;
	printf("input the row number(even):");
	scanf("%d",&step);
	j=step/2;
	//printf("step:%d\n",j);
	array=(int **)malloc(sizeof(int *)*step);
	for(i=0;i<step;i++)
	{
		*(array+i)=(int *)malloc(sizeof(int)*step);
	}
	if(step%2!=0)
	{
		i=0;
		flag=0;
		while(1)
		{
			flag++;
			if(flag==step+1)
			{
				//printf("f=%d\n",flag);
				i=i+2;//赋完值之后被减一，故此处为到当前行的下一行则必须加2
				flag=1;
				j=j-1;
				//printf("i=%d\tj=%d\n",i,j);
			}
			if(i<=-1)
			{
				i=step-1;
			}
			if(j>=step)
			{
				j=0;
			}
			printf("i=%d\tj=%d\tk=%d\n",i,j,k);
			array[i][j]=k;
			i-=1;
			j++;
			k++;
			if(k==step*step+1)
			{
				break;
			}
		}
	}
	else
	{
		printf("wrong input.\n");
		return 0;
	}
	printf("result:\n\n");
	for(i=0;i<step;i++)
	{
		for(j=0;j<step;j++)
		{
			printf("%d\t",array[i][j]);
		}
		printf("\n\n\n");
	}
	for(i=0;i<step;i++)
	{
		free(*(array+i));
	}
	free(array);
	return 0;
}

