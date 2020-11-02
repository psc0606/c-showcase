#include <stdio.h>
#include <curses.h>
//链接	-lcurses 库
int main()
{
	WINDOW *g_win = initscr();
	if (g_win == NULL)
	{
		printf("init screen failed!\n");
		return -1;
	}
	echo();
//	noecho();
	wrefresh(g_win);
	while(1)
	{
		getch();	//注意getchar()不行！
		wrefresh(g_win);
	}
	endwin();
}
