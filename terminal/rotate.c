/*
 * 分别stty -icanon; stty icanon两种终端模式下运行
 */
#include <stdio.h>
#include <ctype.h>

int main()
{
    int c;
    while((c = getchar()) != EOF)
    {
	if(c == 'z') 
	{
	    c = 'a';	
	}
	else if(islower(c))
	{
	    c++;
	}
	putchar(c);
    }
}
