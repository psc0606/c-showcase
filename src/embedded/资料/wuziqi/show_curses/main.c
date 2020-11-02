#include "curses.h"

#define C_ATTR(a, f, b)	((a)|COLOR_PAIR((f)*GCCount+(b)+1))
#define CURRENT_ATTRIBUTE	C_ATTR(g_style, g_fore_color,g_back_color)

extern WINDOW* g_win;
extern GRAPHICS_COLOR g_fore_color;
extern GRAPHICS_COLOR g_back_color;

int main()
{

	init_graphics();
	g_back_color = GCYellow;
	set_attribute();
	wmove(g_win, 10,20);	
	waddch(g_win, 'a');	
	 clear_screen(0, 0, COLS, LINES);
	
	wrefresh(g_win); 
	getchar();
	exit_graphics();

}
