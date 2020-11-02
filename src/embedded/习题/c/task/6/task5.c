#include<stdio.h>
#include<math.h>

int main()
{
	int i;
	float a,k1,k2;
	/*for(a=1;a>=-1;a-=0.1)
	{
		k1=acos(a)*10;
		//k2=((a-3)/4+1)*10;
		for(i=0;i<k1;i++)
			printf(" ");
		printf("*");
		for(;i<62-k1;i++)
			printf(" ");
		printf("*\n");
	}
	for(a=1;a>=-1;a-=0.1)
	{
		printf("\r");
	}
	for(a=1;a>=-1;a-=0.1)
	{
		k2=((a-3)/4+1)*10;
		for(i=0;i<k2;i++)
			printf(" ");
		printf("+\n");
	}*/
	for(a=1;a>=-1;a-=0.1)
	{
		k1=acos(a)*10;
		k2=((a-3)/4+1)*10;
		if(k1<k2)
		{
			for(i=0;i<k1;i++) printf(" ");
			printf("*");
			for(;i<k2;i++) printf(" ");
			printf("+");
			for(;i<62-k1;i++) printf(" ");
			printf("*\n");
		}
		if(k1>k2)
		{
			for(i=0;i<k2;i++) printf(" ");
			printf("+");
			for(;i<k1;i++) printf(" ");
			printf("*");
			for(;i<62-k1;i++) printf(" ");
			printf("*\n");
		}
	}
}
