#include "curses.h"

#define C_ATTR(a, f, b)	((a)|COLOR_PAIR((f)*GCCount+(b)+1))
#define CURRENT_ATTRIBUTE	C_ATTR(g_style, g_fore_color,g_back_color)

extern char *menu1[3];
extern char *menu2[5];
extern WINDOW* g_win;
int main()
{
	init_graphics();
	int sm1 = 0, sm2 = 0;
	
	subm1:	sm1 = submenu1 ();			//对战模式菜单
	
	int flag = 0;
	subm2:	if (flag == 1)
		    {
		    	sm2 = submenu2 ();
			}
	flag = 1;
	mkframe (sm2, 8, 113, 21, 3); //选手框
	chessboard (menu1, sm1);
	
	int ch, mm, conltext, qi, conl_luoqi;
	static int i, j;
	i = 0;		j = 0;
	conltext = 0; //控制选手框中文本颜色变换
	qi = 0;	//控制黑白棋子变换
	conl_luoqi = 1; //控制下子变量
	qipanclear ();			  //清空棋盘	
	chessmove (23, 58);		  //初始化光标
	if (sm1 == 1 && sm2 == 1)			//如果电脑选择人机模式， 并且电脑为黑棋时， 先下
	{
		conl_luoqi = luoqi(qi, 23, 58);
		qi++;									//落子成功后才能使qi++
		mkframetext (conltext++, sm2, 8, 113);
	}  
	while (1)
	{	
		ch = read_key ();
		switch (ch)
		{
			case ' ':
				mm = mainmenu ();
				switch (mm)
				{
					case 0:
						mkdis (29, 112, 12, 5);
						break;
					case 1:
						goto subm1;
					case 2:
						goto subm2;
					case 3:
						if (conl_luoqi != 2)    //胜利后不能悔步
						{
							if (sm1 == 1)							//人机对战时退两步， 因为电脑下得太快， 在你没有做出悔棋就已经多下了一步
							{
								stepback ();
								if (conltext > 0)
									mkframetext (conltext--, sm2, 8, 113);	//选手框也跟着退
							}
							stepback ();
							if (conltext > 0)									//人人对战时退一步
								mkframetext (conltext--, sm2, 8, 113);
						}
						mkdis (29, 112, 12, 5);
						break;
					case 4:
						goto out;
				}
				break;
			case '\r':
				if (conl_luoqi != 2)
				{
					conl_luoqi = luoqi (qi, 23+i*2, 58+j*5);
				}
		       if (conl_luoqi == 1)
			    {
			     	qi++;
		   	       mkframetext (conltext++, sm2, 8, 113);
		   	 	    if (sm1 == 1)
		   	         {
		   	       	conl_luoqi = computer (qi, 8+i, 8+j);  //只有人下成功了， 电脑才会下， 所以在这个if中
		   	       	qi++;
		   	       	mkframetext (conltext++, sm2, 8, 113);
		   	       	 }
	   	        }
	   	       break;
	   	   case KEYUP:
				if (i >= -7)
				{ 
					dischessmove (23+i*2, 58+j*5);
					i--;
				}
				break;
			case KEYLEFT:
				if (j >= -7)
				{
					dischessmove (23+i*2, 58+j*5);
					j--;
				}
				break;
			case KEYDOWN:
				if (i <= 7)
				{
					dischessmove (23+i*2, 58+j*5);
					i++;
				}
				break;
			case KEYRIGHT:
				if (j <= 7)
				{
					dischessmove (23+i*2, 58+j*5);
					j++;
				}
				break;	
		}
		chessmove (23+i*2, 58+j*5);
	}
	out:exit_graphics();
}
