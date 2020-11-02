#ifndef __CURSES__
#define __CURSES__

#include<ncursesw/ncurses.h>
#include<locale.h>


#define C_ATTR(a, f, b)	((a)|COLOR_PAIR((f)*GCCount+(b)+1))

typedef enum{GCBlack,GCRed,GCGreen,GCYellow,GCBlue,
			GCMagenta,GCCyan,GCWhite,GCCount}GRAPHICS_COLOR;


#define KEYUP		0x30ff
#define KEYDOWN		0x31ff
#define KEYLEFT		0x32ff
#define KEYRIGHT   	0x33ff


bool init_graphics(void);	//初始化屏幕函数		
void exit_graphics(void);	//退出屏幕函数

void set_attribute(void);

void clear_screen(int x, int y, int w, int h);	//清屏函数


#endif
