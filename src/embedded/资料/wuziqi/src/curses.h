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


int chessboard (char *p[], int);     //棋台

void mkframe (int, int x, int y, int w, int h);	  //选手框

void mkframetext ();  //选手框内

void zhuose (char *p, GRAPHICS_COLOR color, int x, int y) ;

int makemenu (char *p[], int, int, int, int, int); //菜单制作函数

void mkdis (int, int, int, int);    //覆盖菜单函数

int mainmenu();  //主菜单功能函数

int submenu1 ();  //对战模式菜单

int submenu2 ();  //棋子颜色选择菜单

int chesscolor ();    //棋子颜色菜单

void chessmove (int, int);   //棋子移动

void chessposition (int);  //棋子位置

void dischessmove (); // 消除棋子上一次的位置

int luoqi (int, int, int); //落棋

void qipanclear ();		//清空棋盘函数

void succedinfo (int); //赢棋后棋子闪动

int stepback ();     //悔棋函数

int computer (int, int, int);   //电脑下棋

int chesscirtuation (int, int, int , int );	//估值函数

typedef struct position
{
	int x, y;
	int value;
}POSITION;

#endif
