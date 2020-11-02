#include "curses.h"
#include "public.c"

#define C_ATTR(a, f, b) ((a)|COLOR_PAIR((f)*GCCount+(b)+1))
#define CURRENT_ATTRIBUTE C_ATTR(g_styel, g_fore_color, g_back_color)

int x = 1;									//全局变量，保存菜单返回值
void main()
{
	int i,j;
	while (1)
	{
		switch (x)
		{
			case 0:
				continue_game(i,j);				//当前模式重新开始
				break;
			case 1:
				chess(&i,&j);						//从选择模式开始
				break;
			case 2:
				chess_color(i,&j);				//从选择颜色开始
				break;
			case 4:
				huifang(i,j);
				break;
			case 5:
				break;
		}
		
		if (x == 5)
		{
			break;								//退出游戏
		}
	}
	exit_graphics();
}
