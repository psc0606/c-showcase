#include <stdio.h>
#include <malloc.h>

void main()
{
	int n = 5;
	int **array, i, j;
	array = (int **)malloc(n*sizeof(int *));
	
	for (i = 0; i < n; i++)
	{
		array[i] = (int *)malloc(n*sizeof(int));
		for (j = 0; j < n; j++)
		{
			array[i][j] = i + j + 1;
			printf("%d\t",array[i][j]);
		}
	}
	
	for (i = 0; i < n; i++)
	{
		free(array[i]);
	}
	
	free(array);
}
