#include "curses.h"
#define CHESS "● "
#define HLINE "━ "
#define VLINE "┃"
#define TLC_FRAME	"┏ "
#define TMC_FRAME	"┳ "
#define TRC_FRAME	"┓ "
#define BLC_FRAME	"┗ "
#define BMC_FRAME	"┻ "
#define BRC_FRAME	"┛ "
#define MMC_FRAME	"╋ "
#define MLC_FRAME	"┣ "
#define MRC_FRAME	"┫ "
extern WINDOW* g_win;
extern GRAPHICS_COLOR g_fore_color;
extern GRAPHICS_COLOR g_back_color;
char *menu1[2] = {"People VS People", "People VS Computer"};
char *menu2[5] = {"cancel", "play-style", "chess-color", "step-back", "exit"};
char *menu3[2] = {"black", "white"};
char *menu4[5] = {"opponent", "player", ">", "<", CHESS};
char *menu5[5] = {"computer", "player", ">", "<", CHESS};
char *menu6[3] = {"你胜利了！", "你输了！"};

POSITION 	pos[17*17];	//棋盘上所有的子, 该类型的声明已经放在了头文件中
POSITION 	fiveseq[5];   //五颗棋子连在一起的入栈

typedef struct direction
{
	int h;
	int v;
}DIR;  
DIR dir[8] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}}; //方向扫描

#define KONG 0     //棋盘上为空
#define HEI 1		 //棋盘上为黑棋
#define BAI 2		 //棋盘上为白棋

int qipan[17][17] = {KONG};     //初始化棋盘为空 
int  n, m; //m颜色菜单的选项值， n对战标题的值
int count;     //入栈的下标

/************************************以下是棋台函数的实现*****************************/
int chessboard (char *p[3], int n)
{
	int i, j;						//循环变量
	
	g_back_color = GCWhite;	     	 /**************/
	set_attribute(); //  		    		 棋
	clear_screen (12, 4, 96, 40);//			 台
	g_back_color = GCYellow;	//				 背
	set_attribute ();	//						 景
	clear_screen (10, 3, 96, 40);	/***************/	
									
	mvprintw (5, 51, p[n]);//            棋台标题
	
	for (i = 7; i < 40; i += 2)		/***************/
	{
		for (j = 18; j < 99; j++)//台		画
		{		
			mvprintw (i, j, HLINE);
		}
	}
	for (i = 7; i < 40; i++)
	{
		for (j = 18; j < 100; j += 5)	
		{
			mvprintw (i, j, VLINE);//	   棋
		}
	}
	mvprintw (7, 18, TLC_FRAME);
	for (i = 7, j = 23; j < 95; j += 5)
	{
		mvprintw (i, j, TMC_FRAME);
	}
	mvprintw (7, 98, TRC_FRAME);
	mvprintw (39, 18, BLC_FRAME); //       格
	for (i = 39, j = 23; j < 95; j += 5)
	{
		mvprintw (i, j, BMC_FRAME);
	}
	mvprintw (39, 98, BRC_FRAME);
	wrefresh (g_win);					/***************/
}
/************************************以上是棋台函数的实现*****************************/

/***********************************以下是主菜单函数的实现*****************************/
int mainmenu()			/*主菜单功能函数*/
{
	g_back_color = GCGreen;
	set_attribute();
	mvaddstr (26, 114, "MENU");
	wrefresh(g_win);
	int nm;
	nm = makemenu (menu2, 5, 29, 112, 12, 5); 
	return nm;
}
/***********************************以上是主菜单函数的实现*****************************/

/***************************以下是子菜单玩家模式的选择函数的实现************************/
int submenu1 ()				/*对战模式菜单*/	
{	
	g_back_color = GCGreen;
	set_attribute();
	init_graphics();
	
	mvprintw (16, 55, "Choose play style");
	wrefresh(g_win);
	n = makemenu (menu1, 2, 19, 55, 20, 2);	
	return n; 
}
/***************************以上是子菜单玩家模式的选择函数的实现************************/

/***************************以下是子菜单棋子颜色的选择函数的实现************************/
int submenu2 ()			/*棋子颜色选择菜单*/
{
	g_back_color = GCGreen;  //把背景变量设置成GCGeen, 并初始化一下/整个背景
	set_attribute();
	init_graphics();  

	mvprintw (16, 55, "Choose chess's color");
	wrefresh(g_win);
	m = makemenu (menu3, 2, 19, 55, 20, 2);
	return m;	
}
/***************************以上是子菜单棋子颜色的选择函数的实现************************/

/*********************************以下框架函数的实现***********************************/
void mkframe (int sm2, int x, int y, int w, int h)			
{
	g_back_color = GCWhite;
	set_attribute();   			     
	clear_screen (y, x, w, h);
	g_back_color = GCYellow;
	set_attribute();
	clear_screen (y-2, x-1, w, h);
	switch (sm2)												 /*************/
	{
		case 0:
			zhuose ("player", GCBlack, 8, 114);//					选
			zhuose (menu4[2], GCBlack, x, y+10);//					手	
			zhuose (menu4[3], GCBlack, x+1, 5);//					框
			zhuose (CHESS, GCBlack, x+1, 2);//						的
			break;								//						默
		case 1:								//						认
			zhuose ("opponet", GCBlack, 8, 114); //				显
			zhuose (menu4[2], GCBlack, x, y+10);// 				示
			zhuose (menu4[3], GCBlack, x+1, 5);//					内
			zhuose (CHESS, GCBlack, x+1, 2);//						容
			break;			
	}															 /***************/
}
/*********************************以上框架函数的实现***********************************/

/*********************************以下文本函数的实现***********************************/
void mkframetext (int conltext, int sm2, int x, int y)
{
	if (conltext%2)
	{
		if (sm2 == 0)
			zhuose ("player ", GCBlack, 8, 114);
		else
			zhuose ("opponet", GCBlack, 8, 114);
		zhuose (menu4[2], GCBlack, x, y+10);
		zhuose (menu4[3], GCBlack, x+1, 5);
		zhuose (CHESS, GCBlack, x+1, 2);
	}
	else
	{
		if (sm2 == 0)
			zhuose ("opponet", GCWhite, 8, 114);
		else
			zhuose ("player ", GCWhite, 8, 114);
		zhuose (menu4[2], GCWhite, x, y+10);
		zhuose (menu4[3], GCWhite, x+1, 5);
		zhuose (CHESS, GCWhite, x+1, 2);	
	}
}
/*********************************以上文本函数的实现************************************/

/*******************************以下是涂颜色函数的实现***********************************/
void zhuose (char *p, GRAPHICS_COLOR color, int x, int y)  	//着色
{
	g_fore_color = color;
	set_attribute();	
	mvprintw (x, y, p);
	wrefresh (g_win);
	g_fore_color = GCMagenta;
	set_attribute();	
}
/*******************************以上是涂颜色函数的实现***********************************/

/********************************以下是下棋函数的实现************************************/
int luoqi (int qi, int x, int y)
{
	int i;
	for (i = 0; i < 17*17; i++)									/*************/
	{											   //					   能
		if (pos[i].x == x && pos[i].y == y)  //					  下
		{										  //                      子
			return 0;	//下棋失败                 //                     不
		}
	} 																/***************/
	pos[count].x = x;	   pos[count].y = y;				
	if (x >= 7 && x <= 39 && y >= 18 && y <= 98)	 			/*************/
	{
		if (qi%2)                         		//           接受外界参数控制下子颜色
		{									    					 
			zhuose (CHESS, GCWhite, x, y);  						 
			qipan[(x-7)/2][(y-18)/5] = BAI;                    
		}                                                              
		else                                                   
		{										                        						
			zhuose (CHESS, GCBlack, x, y);  
			qipan[(x-7)/2][(y-18)/5] = HEI;                   /***************/ 
		}                                                           
		if (issucced (x, y, qipan[(x-7)/2][(y-18)/5]))  //     判断是否五子连珠 
		{                                                               
			succedinfo (qipan[(x-7)/2][(y-18)/5]);
			return 2;	//五子连珠
		}
	}
	count++;															
	return 1;	//下棋成功
}
/********************************以上是下棋函数的实现************************************/

/********************************以下是悔步函数的实现************************************/
int stepback ()
{
	int x, y;
	if (count > 0)
	{
		count--;
		x = pos[count].x;
		y = pos[count].y;
		if (x > 7 && x < 39 && y > 18 && y < 98)
		{
			mvprintw (x, y, MMC_FRAME); 
		}
		else if (x == 7)
			  {	
			     if (y > 18 && y < 98)
			        mvprintw (x, y, TMC_FRAME);
			     else if (y == 18)
				  	  mvprintw (x, y, TLC_FRAME);
				  	  else
				  	  {
				  	  	  mvprintw (x, y, TRC_FRAME);
				  	  }
			  }
		     else if (x == 39)
		            {
		            	if (y > 18 && y < 98)
		      	   	   	{
		      	   	   	    mvprintw (x, y, BMC_FRAME);
	      	   	   	    }
	      	   	   	   else if (y == 18)
	      	   	   	    {
	      	   	   	    	mvprintw (x, y, BLC_FRAME);
	      	   	   	    }
	      	   	   	   else
	      	   	   	    {
	      	   	   	   		mvprintw (x, y, BRC_FRAME);
	      	   	   	    }
		            }
		     		else  if (y == 18)
		     				{
		     					mvprintw (x, y, MLC_FRAME);
		     				}
					 		else
					 		{
					 			mvprintw (x, y, MRC_FRAME);
					 		} 		
		qipan[(x-7)/2][(y-18)/5]  = 0;
		pos[count].x = 0;		pos[count].y = 0;
	}
}
/********************************以上是悔步函数的实现************************************/

/****************************以下是判断五子连珠函数的实现*********************************/
int issucced (int x, int y, int hb)
{
	int i, j, k;                //循环变量
	int jilu[4][9];				//记录棋子状态
	int row, col;					//棋台的行列
	int flag = 0;     			//统计相连同色棋子个数
	
	row = (x-7)/2;
	col = (y-18)/5;				//坐标转换
	for (i = 0, j = -4; j < 5; j++)
	{
		if (row + j >= 0 && row +j <= 16)
		{
			jilu[i][j + 4] = qipan[row+j][col];
		}
		else
		{
			jilu[i][j + 4] = KONG;
		}
	}
	for (i = -1; i < 2; i++)
	{
		for (j = -4; j < 5; j++)
		{
			if (row+i >= 0 && row + i <= 16 && col + j >= 0 && col +j <= 16)
			{
				jilu[2+i][4+j] = qipan[row + i*j][col + j];
			}
			else
			{
				jilu[2+i][4+j] = KONG;
			}
		}
	}
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 5; j++)
		{
			for (k = j; k < j+5; k++)
			{
				if (jilu[i][k] != hb)
				{
					break;
				}
				flag++;
			}
			if (flag == 5)
			{  
	         /****************以下找出五子连珠的坐标*******************/		
				if (i == 0)									//找出五颗棋子的坐标
				{
					for (k = j; k < j+5; k++)
					{
						fiveseq[k-j].x = x+(k-4)*2;
						fiveseq[k-j].y = y;
					}
				} 
				else
				{   
					for (k = j; k < j+5; k++)
					{
						fiveseq[k-j].x = x+2*(k-4)*(i-2);
						fiveseq[k-j].y = y+5*(k-4);
					}
				}
			 /****************以上找出五子连珠的坐标*********************/	
				return 1;
			}
			flag = 0;
		}
	}
	return 0;
}
/****************************以上是判断五子连珠函数的实现*********************************/

/*************************以下是五子连珠后信息框函数的实现********************************/
void succedinfo (int hb)
{
	int i, j;
	for (i = 0; i < 5; i++)
	{
		zhuose(CHESS, GCRed, fiveseq[i].x, fiveseq[i].y);
		usleep (200000);												  //延时按顺序变化颜色
	}
	for (j = 0; j < 3; j++)
	{
		for (i = 0; i < 5; i++)
		{
			zhuose(CHESS, GCGreen, fiveseq[i].x, fiveseq[i].y);    //全部变颜色
		}
		usleep (200000);
		for (i = 0; i < 5; i++)
		{
			zhuose(CHESS, GCRed, fiveseq[i].x, fiveseq[i].y);    //全部变颜色
		}
		usleep (300000);
	}
	for (i = 0; i < 5; i++)
	{
		fiveseq[i].x = 0;	   fiveseq[i].y = 0;	                  //清空
	}
	if (hb == m+1)
	{
		zhuose (menu6[0], GCRed, 22, 55);
	}
	else
	{
		zhuose (menu6[1], GCRed, 22, 56);
	}
}
/*************************以上是五子连珠后信息框函数的实现********************************/

/**************************以下是清空棋盘和棋栈函数的实现*********************************/
void qipanclear ()					
{	
	int i, j;
	for (i = 0; i <= 16; i++)
	{
		for (j = 0; j <= 16; j++)
		{
			qipan[i][j] = KONG;
		}
	}
	for (i = 0; i < 17*17; i++)
	{
		pos[i].x = 0;
		pos[i].y = 0;
	}
}
/*************************以上是清空棋盘和棋栈函数的实现**********************************/

/*************************以下是光标移动函数的实现****************************************/
void chessmove (int x, int y)
{
	g_fore_color = GCRed;
	set_attribute();
	mvprintw (x-1, y-2, TLC_FRAME);
	mvprintw (x-1, y+2, TRC_FRAME);
	mvprintw (x+1, y-2, BLC_FRAME);
	mvprintw (x+1, y+2, BRC_FRAME);
	wrefresh (g_win);
	g_fore_color = GCMagenta;
	set_attribute();	
}
/*************************以上是光标移动函数的实现****************************************/

/*************************以下是消除光标痕迹函数的实现************************************/
void dischessmove (int x, int y)
{
	mvprintw (x-1, y-2, " ");
	mvprintw (x-1, y+2, " ");
	mvprintw (x+1, y-2, " ");
	mvprintw (x+1, y+2, " ");
}
/*************************以上是是消除光标痕迹函数的实现***********************************/

/*****************************以下是覆盖菜单函数的实现************************************/
void mkdis (int x, int y, int w, int h)		
{
	g_back_color = GCGreen;
	set_attribute();   			     
	clear_screen (y - 1, x - 3, w + 1, h + 3);
	
	g_back_color = GCYellow; 	//还原
	set_attribute ();
}
/*****************************以上是覆盖菜单函数的实现************************************/

/*****************************以下是制作菜单函数的实现************************************/
int makemenu (char *p[], int n, int x, int y, int w, int h)	
{	
	int ch, i;
	g_back_color = GCWhite;					/**********/
	set_attribute();            //				 菜
	clear_screen (y, x, w, h);  //				 单
	g_back_color = GCYellow;    //				 背
	set_attribute ();           //				 景
	clear_screen (y - 1, x - 1, w, h);    /**********/
	
	for (i = 0; i < n; i++)
	{
		mvprintw (x+i -1, y, p[i]);
	}
	i = 0;
	g_fore_color = GCBlue;	   //    进入菜单后的选项颜色
	set_attribute ();
	mvprintw (x+i-1, y, p[i]);
	wrefresh(g_win); 	
	while (1)
	{
		switch (ch)
		{
			case KEYUP:
			case KEYLEFT:
				mvprintw (x+i-1, y, p[i]);
				i--;	
				i = (i+n)%n;
				g_fore_color = GCBlue;
				set_attribute ();
				mvprintw (x+i-1, y, p[i]);	
				break;
			case KEYDOWN:
			case KEYRIGHT:
				mvprintw (x+i-1, y, p[i]);
				i++;
				i = (i+n)%n;
				g_fore_color = GCBlue;
				set_attribute ();
				mvprintw (x+i-1, y, p[i]);
				break;
		}
		wrefresh(g_win); 
		ch = read_key ();
		g_fore_color = GCMagenta;
		set_attribute ();
		wrefresh(g_win);
		if (ch == '\r')
		{
			return i;
		}
	}
}
/*****************************以上是制作菜单函数的实现************************************/

/*****************************以下是电脑下棋函数的实现************************************/
int computer (int qi, int cr, int cc)
{
	int i, j, count = 0;
	POSITION d[17*17], a[17*17], best, bd, ba;  //bd ba 最佳防御点
	
	for (i = 0; i < 17; i++)	
	{
		for (j = 0; j < 17; j++)
		{
			if (qipan[i][j] == KONG)
			{
				if (m == 0)
				{
					d[count].value = chesscirtuation (1, 2, i, j);
					a[count].value = chesscirtuation (2, 1, i, j);
				}
				else
				{
					d[count].value = chesscirtuation (2, 1, i, j);
					a[count].value = chesscirtuation (1, 2, i, j);
				}
				d[count].x = i;	d[count].y = j;
				a[count].x = i;	a[count].y = j;				
				count++;
			}
		}
	}
	bd = d[0];		ba = a[0];
	for (i = 0; i < count; i++)
	{
		if (bd.value < d[i].value)
		{
			bd = d[i];
		}
		if (ba.value < a[i].value)
		{
			ba = a[i];
		}	
	}	
	best = (ba.value >= bd.value)?ba:bd;   //玩家选白棋， 电脑首选进攻
	return luoqi(qi, best.x*2+7, best.y*5+18);																								
}  
/*****************************以上是电脑下棋函数的实现************************************/

/********************************以下是棋局判断函数的实现*********************************/
int chesscirtuation (int color1, int color2, int cr, int cc)
{
	int ls, rs; 	 //棋局的两端棋位状况：空位0， 越界或color2:3
	int r, c;		 //行列
	int score = 0; //得分
	int i, j, k = 0, flag;  // 统计同色数数目
	while (k < 4)
	{
		r = cr;	c = cc;
		flag = 1;
		for (i = 0; i < 4; i++)
		{
			r += dir[k].h;	c += dir[k].v;
		   if (r >= 0 && r <= 16 && c >= 0 && c <= 16 && qipan[r][c] == color1)
			{
				flag++;
			}
			else 
			{
				break;
			}     
		}
		if (r < 0 || r > 16 || c < 0 || c > 16 || qipan[r][c] == color2)
		{
			ls = 3; 
		}
		else
		{
			ls = 0;
		}
		
		r = cr;	c = cc;
		for (i = 0; i < 4; i++)
		{
			r -= dir[k].h;	c -= dir[k].v;
		   if (r >= 0 && r <= 16 && c >= 0 && c <= 16 && qipan[r][c] == color1)
			{
				flag++;
			}
			else 
			{
				break;
			}
		}
		if (r < 0 || r > 16 || c < 0 || c > 16 || qipan[r][c] == color2)
		{
			rs = 3;
		}
		else
		{
			rs = 0;
		}
		k++;
		score += score_len (flag, ls, rs);
	}
	return score;
}
/********************************以上是棋局判断函数的实现*********************************/

/**********************************以下是评分函数的实现**********************************/
int score_len(int l,int i,int j)							//评分
{
	if (i == 0 && j == 0)
	{
		switch (l)
		{
			case 1:
				return 2;
			case 2:
				return 4;
			case 3:
				return 15;
			case 4:
				return 40;
			default:
				return 100;
		}
	}
	else if (i == 3 || j == 3)
	{
		switch (l)
		{
			case 1:
			case 2:
			case 3:
			case 4:
				return 1;
			default:
				return 100;
		}
	}
	else if (i == 0 || j == 0)
	{
		switch (l)
		{
			case 1:
				return 3;
			case 2:
				return 5;
			case 3:
				return 6;
			case 4:
				return 20;
			default:
				return 100;
		}
	}
	else
	{
		switch (l)
		{
			case 5:
				return 100;
			default:
				return 0;
		}
	}
}
/**********************************以上是评分函数的实现**********************************/
