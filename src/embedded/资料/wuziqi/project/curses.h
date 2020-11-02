#ifndef _CURSES_
#define _CURSES_

#include <ncursesw/ncurses.h>
#include <locale.h>

#define C_ATTR(a, f, b) ((a)|COLOR_PAIR((f)*GCCount+(b)+1))

typedef enum{GCBlack, GCRed, GCGreen, GCYellow, GCBlue,
			GCMagenta, GCCyan, GCWhite, GCCount}GRAPHICS_COLOR;

typedef struct
{
	int i;
	int j;
}Node;																				//记录下棋的坐标

typedef struct
{
	int score;																		//每个点的评分
	int x;
	int y;
}Cnode;																				//记录电脑判断的坐标
			
#define KEYUP 			0x30ff
#define KEYDOWN		0x31ff
#define KEYLEFT		0x32ff
#define KEYRIGHT		0x33ff

bool init_graphics(void);															//初始化屏幕函数
void exit_graphics(void);															//退出屏幕函数
void clear_screen(int x, int y, int w, int h);									//清屏

void set_attribute(void);

void init_background();															//初始化背景

void chess(int *i,int *j);														//开始游戏

void slect_moshi(int *i);															//选择模式

void slect_qizi(int *j);															//选择棋子

void chess_color(int i,int *j);													//选择棋子颜色

void table_qipan(int m);															//画棋盘

void table_player(int i,int j);													//选手提示菜单

void slect_menu(int *n);															//选择菜单

void continue_game(int i,int j);													//继续游戏

void people(int a[19][19],int j);												//人人对战移动光标

void computer(int a[19][19],int j);												//人机对战移动光标

void move_cursor(int n,int *i,int *j);											//移动光标

void delete_cursor(int i,int j);													//擦除光标
	
void set_chess(int a[19][19],int,int,int,Node seat[290]);						//对战落子

void people_chess(int j);															//人人对战

void set_seat(int i, int j, int n, Node seat[290]);							//记录落子坐标

void regret_chess(int i,int j,int *n,Node seat[290],int a[19][19]);			//悔棋

void exchange_player(int n,int k);												//交换显示轮到下棋的选手

void computer_chess(int j);														//人机对战

void computer_judge(int a[19][19],int p,int *x,int *y);						//找出x,y为下棋位置

void judge_seat(int a[19][19],int n,int i, int j,Cnode x[289],int k);		//记录坐标及评分

void score_max(int a[19][19],Cnode n[289],int p,int *k);						//攻防评分

int score_len(int i,int j,int l);												//评分

void change_red(int i,int j,int l,int k,int n);								//胜利闪动棋子

int is_win(int a[19][19],int n);													//判断是否胜利

void display_win(int a[19][19],int k);											//显示胜利

#endif
