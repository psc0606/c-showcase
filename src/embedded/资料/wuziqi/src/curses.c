#include <fcntl.h>
#include <unistd.h>
#include "curses.h"
#define CURRENT_ATTRIBUTE	C_ATTR(g_style, g_fore_color,g_back_color)

WINDOW* g_win = NULL;
GRAPHICS_COLOR g_fore_color = GCMagenta;
GRAPHICS_COLOR g_back_color = GCGreen;

int g_style = A_BOLD;

//初始化屏幕函数
bool init_graphics(void)
{
	setlocale(LC_ALL, "");
	
    g_win=initscr();     
	if(g_win==NULL)
	{
		printf("call initscr() failed!\n");
		return false;
	}
	cbreak();//cbreak()模式开启后，除delete或ctrl等仍被视为特殊字元外,一切字元立刻被读取。nocbreak是关闭
	noecho();//用来控制输入的字元是否显示在终端上，系统预设是开启。echo是开启
	
	//keypad(g_win,TRUE);	//启用curses库中对键盘特殊键的定义

    if(has_colors())        //has_colors函数检测你的屏幕的颜色显示能力
	 {
		int i,j,k;
      	start_color();  //要启动彩色机制，必须先调用start_color()函数
		for(i=0, k=0; i<GCCount; i++)
		{
			for(j=0; j<GCCount; j++)
			{
				init_pair(++k, i, j);  //初始化所有颜色
			}
		}
	 }

    curs_set(0);                      //隐藏鼠标
    clear_screen(0, 0, COLS, LINES);  //绘制背景
    wrefresh(g_win);                  //刷新缓冲区
	 return true;
	
}

//退出屏幕函数
void exit_graphics(void)
{
	endwin();
}

void set_attribute()
{
	wattrset(g_win, CURRENT_ATTRIBUTE);
}

//清屏函数
void clear_screen(int x, int y, int w, int h)
{
	int i, j;
	wattrset(g_win, CURRENT_ATTRIBUTE);     //更新前景色和背景色	
	for(i=0; i<h; i++)
	{
		wmove(g_win, i+y, x);				//移动光标
		for(j=0; j<w; j++)
		{
			waddch(g_win, ' ');				//在当前光标位置输出一个字符
		}
	}
	wrefresh(g_win);
}

int read_key()
{
	int n, flags;
	int ch;
	ch=getc(stdin);
	if(ch==27)
	{
		flags=fcntl(STDIN_FILENO, F_GETFL, 0);// fcntl():stdin_fileno代表要设置的文件。F_GETFL取得文件描述词状态旗标，来自open()中的flag。
		fcntl(STDIN_FILENO, F_SETFL, flags|O_NONBLOCK);
		ch=getc(stdin);
		if(ch==91)
		{
			ch=getc(stdin);
			switch(ch)
			{
			case 'D':
			case 'd':
				ch=KEYLEFT;
				break;
			case 'C':
			case 'c':
				ch=KEYRIGHT;
				break;
			case 'B':
			case 'b':
				ch=KEYDOWN;
				break;
			case 'A':
				ch=KEYUP;
				break;
			default:
				ch=0;
			}
		}
		else
		{
			ch=27;
		}
		fcntl(STDIN_FILENO, F_SETFL, flags);
	}
	return ch;
}
