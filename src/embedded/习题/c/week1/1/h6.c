/*----------------------------------------
**	编写一个C程序完成如下动画输出
**	fflush(stdout)语句可以将输出内容立刻在屏
**	幕上显示出来；usleep(150000)语句可以让电
**	脑睡眠0.15秒；灵活应用退格字符’\b’』
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	while(1)
		{
			printf("A");
			fflush(stdout);
			usleep(150000);
			printf("\b");
			printf("B");
			fflush(stdout);
			usleep(150000);
			printf("\b");
			printf("C");
			fflush(stdout);
			usleep(150000);
			printf("\b");
			printf("*");
		}
}
