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
	cbreak();
//	nocbreak();
	wrefresh(g_win);
	getchar();
	endwin();
}
