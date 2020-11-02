/*----------------------------------------
**	3.利用main函数可以读取命令行参数，建立文件
**	like.c.当命令行有两个参数时，例如
**	like football，输出Do you like football？
**  当命令行有3个参数时，例如like football basketball 时，**	输出Do you like football or basketball？
**	当命令行有4个以上参数时，例如like C Vb Foxpro Java时
**	输出Do you like C，Vb，Foxpro or Java？
**----------------------------------------
*/

#include <stdio.h>
void main(int argc,char **argv)
{
	int i;
	switch (argc)
	{
		case 1:
			break;
		case 2:
			printf("Do you like %s？\n",*(argv+(--argc)));
			break;
		default:
			printf("Do you like");
			for (i = 0; i < argc-1; i++)
			{
				printf("%s,",*(argv+i));
			}
			printf("or %s ？\n",*(argv+i));
			break;
	}
}
