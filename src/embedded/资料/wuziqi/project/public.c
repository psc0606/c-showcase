#include "curses.h"

extern WINDOW *g_win;
extern GRAPHICS_COLOR g_fore_color;
extern GRAPHICS_COLOR g_back_color;
extern int x;											//X,Y定义为外部全局变量

/**********************************公有函数*****************************************/

void chess(int *i,int *j)
{
	init_background();									//初始化背景
	slect_moshi(i);										//初始化背景后进入选择模式菜单
	slect_qizi(j);											//初始化背景后进入选择棋子菜单
	if (*i == 0)
	{
		people_chess(*j);									//人人对战
	}
	else
	{
		computer_chess(*j);								//人机对战
	}
}

void chess_color(int i,int *j)
{
	init_background();									//初始化背景
	slect_qizi(j);											//初始化背景后进入选择棋子菜单
	if (i == 0)
	{
		people_chess(*j);									//人人对战
	}
	else
	{
		computer_chess(*j);								//人机对战
	}
}

void continue_game(int i,int j)							//继续游戏
{
	init_background();
	if (i == 0)
	{
		people_chess(j);									//人人对战
	}
	else
	{
		computer_chess(j);								//人机对战
	}
}

void printf_str(int fore,int back,int y,int x,char *str)	//打印字符串
{
	g_fore_color = fore;
	g_back_color = back;
	set_attribute();
	mvprintw(y,x,str);
}

void set_color(int fore,int back)						//设置颜色
{
	g_fore_color = fore;
	g_back_color = back;
	set_attribute();
}

void init_background()									//初始化背景
{
	init_graphics();
	set_color(GCCyan,GCCyan);
	clear_screen(0, 0, COLS, LINES);
}

void slect_moshi(int *i)									//选择模式菜单
{
	set_color(GCCyan,GCBlack);
	clear_screen(70, 20, 15, 3);
	set_color(GCCyan,GCBlue);
	clear_screen(68, 19, 15, 3); 
	mvprintw(19,68,"请选择游戏模式:");
	
	int ch = KEYUP;
	while (1)
	{
		switch (ch)
		{
			case KEYUP:
			case KEYLEFT:
				printf_str(GCCyan,GCBlue,21,70,"人与电脑对战");
				printf_str(GCCyan,GCYellow,20,70,"人与人对战");
				*i = 0;
				break;
			case KEYDOWN:
			case KEYRIGHT:
				printf_str(GCCyan,GCBlue,20,70,"人与人对战");
				printf_str(GCCyan,GCYellow,21,70,"人与电脑对战");
				*i = 1;
				break;
		}
		wrefresh(g_win); 
		ch = read_key();
		if (ch == '\r')
		{
			break;
		}
	}
}

void slect_qizi(int *j)									//选择棋子菜单
{
	set_color(GCCyan,GCBlack);
	clear_screen(70, 20, 15, 3);
	set_color(GCCyan,GCBlue);
	clear_screen(68, 19, 15, 3); 
	mvprintw(19,68,"请选择游戏棋子:");
	
	int ch = KEYUP;
	while (1)
	{
		switch (ch)
		{
			case KEYUP:
			case KEYLEFT:
				printf_str(GCCyan,GCBlue,21,70,"黑   棋");
				printf_str(GCCyan,GCYellow,20,70,"白   棋");
				*j = 1;
				break;
			case KEYDOWN:
			case KEYRIGHT:
				printf_str(GCCyan,GCBlue,20,70,"白   棋");
				printf_str(GCCyan,GCYellow,21,70,"黑   棋");
				*j = 2;
				break;
		}
		wrefresh(g_win); 
		ch = read_key();
		if (ch == '\r')
		{
			break;
		}
	}
}

void table_qipan(int m)									//画棋盘
{
	int i, j;
	set_color(GCCyan,GCBlack);
	clear_screen(6, 3, 92, 39);
	set_color(GCBlue,GCYellow);
	clear_screen(4, 2, 92, 39);
	set_color(GCYellow,GCYellow);
	
	for (i = 0; i < 33; i++)
	{
		if (i == 0)
		{
			for (j = 1; j < 72; j = j + 5)
			{
				mvprintw(5, 9+j, "━━━━┳");
			}
			mvprintw(5, 9, "┏");
			mvprintw(5, 86, "━━━┓");
		}
		else if (i == 32)
		{
			for (j = 1; j < 72; j = j+5)
			{
				mvprintw(37, 9+j, "━━━━┻");
			}
			mvprintw(37, 9, "┗");
			mvprintw(37, 86, "━━━┛");
		}
		else if (i % 2 == 1)
		{
			for (j = 0; j < 81; j = j + 5)
			{
				mvprintw(5+i, 9+j, "┃");
			}
		}
		else
		{
			for (j = 1; j < 72; j = j + 5)
			{
				mvprintw(5+i, 9+j, "━━━━╋");
			}
			mvprintw(5+i, 86, "━━━┫");
			mvprintw(5+i, 9, "┣");
		}
	}
}

void table_player(int i,int j)							//选手提示菜单
{
	printf_str(GCBlue,GCCyan,27,105,"游戏菜单:(按space进入)");
	
	if (i == 1)											//i = 1为人机，j = 1为人选白棋
	{
		printf_str(GCBlue,GCYellow,3, 40,"人与电脑对战");
		printf_str(GCRed,GCCyan,6,104,"★ 选   手");
		printf_str(GCBlue,GCCyan,8,104,"  电   脑");
	}
	else
	{
		printf_str(GCBlue,GCYellow,3, 40,"人与人对战");
		if (j == 1)
		{
			printf_str(GCBlue,GCCyan,6, 104,"  选   手1");
			printf_str(GCRed,GCCyan,8,104,"★ 选   手2");
		}
		else
		{
			printf_str(GCRed,GCCyan,6,104,"★ 选   手1");
			printf_str(GCBlue,GCCyan,8,104,"  选   手2");	
		}
	}
	if (j == 1)
	{
		printf_str(GCWhite,GCCyan,6,115,"> ● <");
		printf_str(GCBlack,GCCyan,8, 115,"> ● <");
	}
	else
	{
		printf_str(GCBlack,GCCyan,6,115,"> ● <");
		printf_str(GCWhite,GCCyan,8, 115,"> ● <");
	}
}

void slect_menu(int *n)									//选择菜单
{
	int ch = KEYUP;
	while (1)
	{
		set_color(GCGreen,GCCyan);
		
		mvprintw(28,105,"继续游戏");
		mvprintw(29,105,"选择游戏模式");
		mvprintw(30,105,"选择游戏棋子");
		mvprintw(31,105,"悔	棋");
		mvprintw(32,105,"退	出"); 
		switch (ch)
		{
			case KEYUP:
			case KEYLEFT:
				(*n)--;
				break;
			case KEYDOWN:
			case KEYRIGHT:
				(*n)++;
				break;
			default:
				break;
		}
		set_color(GCYellow,GCCyan);
		
		switch ((*n) = (((*n) == -1) ? 4:(*n)) % 5)		//光标循环移动
		{
			case 0:
				mvprintw(28,105,"继续游戏");
				break;
			case 1:
				mvprintw(29,105,"选择游戏模式");
				break;
			case 2:
				mvprintw(30,105,"选择游戏棋子");
				break;
			case 3:
				mvprintw(31,105,"悔	棋");
				break;
			case 4:
				mvprintw(32,105,"退	出");
				break;	
			default:
				break;
		}
		wrefresh(g_win);
		ch = read_key();
		if (ch == ' ' || ch == '\r')
		{
			if (ch == ' ')
			{
				(*n) = 0;									//退出菜单
			}
			clear_screen(105, 28, 15, 5); 
			break;
		} 
	}
}

void move_cursor(int n,int *i,int *j)
{
	if (n % 2 == 0)
	{
		set_color(GCBlack,GCYellow);
	}
	else
	{
		set_color(GCWhite,GCYellow);
	}
	
	(*i) = ((*i) <= 17)? (*i):1;
	(*i) = ((*i) >= 1)? (*i):17;
	(*j) = ((*j) <= 17)? (*j):1;
	(*j) = ((*j) >= 1)? (*j):17;
	mvprintw(2+2*(*i), 2+5*(*j), "┏");
	mvprintw(2+2*(*i), 6+5*(*j), "┓");
	mvprintw(4+2*(*i), 6+5*(*j), "┛");
	mvprintw(4+2*(*i), 2+5*(*j), "┗");
	wrefresh(g_win);
}

void delete_cursor(int i,int j)							//擦除光标
{
	mvprintw(2+2*i, 2+5*j, " ");
	mvprintw(2+2*i, 6+5*j, " ");
	mvprintw(4+2*i, 6+5*j, " ");
	mvprintw(4+2*i, 2+5*j, " ");
}

void set_chess(int a[19][19], int i, int j, int n, Node seat[290])		//落子！
{
	if (n % 2 == 1)
	{
		a[i][j] = 2;					//黑棋为2
		printf_str(GCBlack,GCYellow,3+2*i, 4+5*j, "● ");
	}
	else
	{
		a[i][j] = 1;					//白棋为1
		printf_str(GCWhite,GCYellow,3+2*i, 4+5*j, "● ");
	}
	wrefresh(g_win);
	set_seat(i,j,n,seat);									//记录落子坐标
}

int is_win(int a[19][19],int n)							//判断是否胜利
{
	int i, j, k = (n % 2 == 0)? 1: 2;
	int n1 = 0,n2 = 0,n3 = 0,n4 = 0,l;
	for (i = 1; i < 18; i++)
	{
		for (j = 1; j < 18; j++)
		{
			if (a[i][j] == k)
			{
				for (l = 0; a[i][j+l] == k; l++);						//水平方向
				if (l >= 5 && n1 == 0)
				{
					n1 = 1;
					change_red(i,j,l,k,1);								//闪红色
				}		

				for (l = 0; a[i+l][j] == k; l++);						//垂直方向
				if (l >= 5 && n2 == 0)
				{
					n2 = 1;
					change_red(i,j,l,k,2);								//闪红色
				}

				for (l = 0; a[i+l][j+l] == k; l++);						//右斜方向
				if (l >= 5 && n3 == 0)
				{
					n3 = 1;
					change_red(i,j,l,k,3);								//闪红色
				}

				for (l = 0; a[i+l][j-l] == k; l++);						//左斜方向
				if (l >= 5 && n4 == 0)
				{
					n4 = 1;
					change_red(i,j,l,k,4);								//闪红色
				}
			}
		}
	}
	if (n1 || n2 || n3 || n4)
	{
		return k;
	}
	else
	{
		return 0;
	}
}

void change_red(int i,int j,int l,int k,int n)							//胜利闪动棋子
{
	int p, m = 0;
	while ((++m) != 6)
	{
		if (m % 2 == 1)
		{
			set_color(GCRed,GCYellow);
		}
		else if (k == 1)
		{
			set_color(GCWhite,GCYellow);
		}
		else
		{
			set_color(GCBlack,GCYellow);
		}
		
		for (p = 0; p < l; p++)
		{
			if (n == 1)
			{
				mvprintw(3+2*i, 4+5*(j+p), "● ");
			}
			else if (n == 2)
			{
				mvprintw(3+2*(i+p), 4+5*j, "● ");
			}
			else if (n == 3)
			{
				mvprintw(3+2*(i+p), 4+5*(j+p), "● ");
			}
			else if (n == 4)
			{
				mvprintw(3+2*(i+p), 4+5*(j-p), "● ");
			}
		}
		wrefresh(g_win);
		usleep(300000);
	}
}
	
void display_win(int a[19][19],int k)							//显示胜利
{
	set_color(GCGreen,GCBlack);
	clear_screen(106, 16, 15, 4);
	set_color(GCGreen,GCBlue);
	clear_screen(104, 15, 15, 4);
	if (k == 1)
	{
		mvprintw(17,105,"恭喜白棋胜利!");
	}
	else
	{
		mvprintw(17,105,"恭喜黑棋胜利!");
	}
	while (1)
	{
		int n = 1;
		slect_menu(&n);
		switch (n)
		{
			case 0:
				x = 3;														//继续游戏
				return;
			case 1:
				x = 1;														//选择模式
				return;
			case 2:
				x = 2;														//选择棋子
				return;
			case 3:
				break;
			case 4:
				x = 0;														//退出游戏
				return;
			default:
				break;
		}
	}
}

void set_seat(int i,int j,int n, Node seat[290])					//记录每步棋坐标
{
	seat[n].i = i;
	seat[n].j = j;
}

void regret_chess(int i,int j,int *n,Node seat[290],int a[19][19])	//悔棋
{
	if (i == 1 && j == 1 && (*n) == 1)
	{
		return;
	}
	else if ((*n) == 0)
	{
		return;
	}
	else
	{
		set_color(GCYellow,GCYellow);
		if (seat[*n].i == 1 && seat[*n].j == 1)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┏ ");
		}
		else if (seat[*n].i == 1 && seat[*n].j == 17)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┓ ");
		}
		else if (seat[*n].i == 17 && seat[*n].j == 1)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┗ ");
		}
		else if (seat[*n].i == 17 && seat[*n].j == 17)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┛ ");
		}
		else if (seat[*n].i == 1)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┳ ");
		}
		else if (seat[*n].i == 17)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┻ ");
		}
		else if (seat[*n].j == 1)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┣ ");
		}
		else if (seat[*n].j == 17)
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "┫ ");
		}
		else
		{
			mvprintw(3+2*seat[*n].i, 4+5*seat[*n].j, "╋ ");
		}
		a[seat[*n].i][seat[*n].j] = 0;
		(*n)--;
	}
}

/*******************************人人对战***********************************************/

void people_chess(int j)												//人人对战
{
	int i,a[19][19] = {0};
	for (i = 0; i < 19; i++)											//外围一圈的3为墙，防止越界
	{
		a[0][i] = 3;
		a[i][0] = 3;
		a[18][i] = 3;
		a[i][18] = 3;
	}
	table_qipan(0);														//初始化棋盘
	table_player(0,j);													//选择模式后画出棋盘
	people(a,j);
}

void people(int a[19][19],int k)	
{
	int ch, n = 0, m;
	int i = 9, j = 9;
	Node seat[290];
	while (1)
	{
		move_cursor(n,&i,&j);												//移动光标
		delete_cursor(i,j);												//擦除光标
		
		ch = read_key();
		switch (ch)
		{
			case KEYUP:
				i--;
				break;
			case KEYLEFT:
				j--;
				break;
			case KEYDOWN:
				i++;
				break;
			case KEYRIGHT:
				j++;
				break;
			default:
				break;
		}
		if (ch == ' ')														//选项菜单
		{
			int l = 1;
			slect_menu(&l);
			switch (l)
			{
				case 0:
					break;
				case 1:
					x = 1;													//选择模式
					return;
				case 2:
					x = 2;													//选择棋子
					return;
				case 3:
					regret_chess(0,0,&n,seat,a);							//悔棋
					break;
				case 4:
					x = 0;													//退出游戏
					return;
				default:
					break;
			}
		}
		if (ch == '\r' && a[i][j] == 0)
		{
			set_chess(a,i,j,++n,seat);									//落子
			exchange_player(n,k);
			if (m = is_win(a,n))
			{
				display_win(a,m);											//判断是否胜利
				return;
			}
		}
	}
}

void exchange_player(int n,int k)										//交换显示轮到下棋的选手
{
	printf_str(GCBlue,GCCyan,6,104,"  选   手1");
	printf_str(GCBlue,GCCyan,8,104,"  选   手2");
	set_color(GCRed,GCCyan);
	
	if (n % 2 == k - 1)
	{
		mvprintw(8, 104,"★ 选   手2");
	}
	else
	{
		mvprintw(6, 104,"★ 选   手1");
	}
}

/*******************************人机对战**********************************************/

void computer_chess(int j)
{
	int i,a[19][19] = {0};
	for (i = 0; i < 19; i++)					//外围一圈的3为墙，防止越界
	{
		a[0][i] = 3;
		a[i][0] = 3;
		a[18][i] = 3;
		a[i][18] = 3;
	}
	table_qipan(1);							//初始化棋盘
	table_player(1,j);						//选择模式后画出棋盘
	computer(a,j);								//移动光标
}

void computer(int a[19][19],int k)			//移动光标,k == 1，选手为白棋
{
	int ch, n = 0, m;
	int i = 9, j = 9, x1, y1;
	Node seat[290];
	
	if (k == 1)										//当选择白棋时，在9，9下黑棋
	{
		set_chess(a,i,j,++n,seat);
	}
	while (1)
	{
		move_cursor(n,&i,&j);
		delete_cursor(i,j);
		
		ch = read_key();
		switch (ch)
		{
			case KEYUP:
				i--;
				break;
			case KEYLEFT:
				j--;
				break;
			case KEYDOWN:
				i++;
				break;
			case KEYRIGHT:
				j++;
				break;
			default:
				break;
		}
		if (ch == ' ')												//选项菜单
		{
			int m = 1;
			slect_menu(&m);
			switch (m)
			{
				case 0:
					break;
				case 1:
					x = 1;											//选择模式
					return;
				case 2:
					x = 2;											//选择棋子颜色
					return;
				case 3:
					regret_chess(1,k,&n,seat,a);					//悔棋
					regret_chess(1,k,&n,seat,a);
					break;
				case 4:
					x = 0;											//退出游戏
					return;
				default:
					break;
			}
		}
		if (ch == '\r' && a[i][j] == 0)
		{
			set_chess(a,i,j,++n,seat);							//人下棋
			if (m = is_win(a,n))									//判断输赢
			{
				display_win(a,m);
				return;
			}
		}
		if (n % 2 == k-1)
		{
			computer_judge(a,k,&x1,&y1);							//找出x,y为下棋位置
			set_chess(a,x1,y1,++n,seat);							//落子
			if (m = is_win(a,n))									//判断输赢
			{
				display_win(a,m);
				return;
			}
		}
	}
}

void computer_judge(int a[19][19],int p,int *x,int *y)
{
	int m = 0,n = 0;
	int q = (p == 1) ? 2 : 1;								//人选白棋时，p = 1，q = 2
	Cnode b[289],c[289],d[289];							//人选黑棋时，p = 2，q = 1
	score_max(a,b,p,&m);
	score_max(a,c,q,&n);
	
	if (c[n].score >= b[m].score || (c[n].score > 100) ||(c[n].score > 45 && b[m].score < 100))
																		//比较防守和攻击的最大值
	{
		(*x) = c[n].x;
		(*y) = c[n].y;
	}
	else 
	{
		(*x) = b[m].x;
		(*y) = b[m].y;
	}
}

void score_max(int a[19][19],Cnode node[289],int p,int *n)
{
	int i, j, Max, m = 0;
	for (i = 1; i < 18; i++)
	{
		for (j = 1; j < 18; j++)
		{
			if (a[i][j] == 0)
			{
				a[i][j] = p;
				node[m].x = i;
				node[m].y = j;
				judge_seat(a,m++,i,j,node,p);			//防守计算评分
				a[i][j] = 0;
			}
		}
	}
	Max = node[0].score;								//找出评分最大值的下标
	for (i = 0; i < m; i++)
	{
		if (Max < node[i].score)
		{
			Max = node[i].score;
			*n = i;
		}
	}
}

void judge_seat(int a[19][19],int n,int i, int j,Cnode x[289],int k)		//判断每个点的4个方向
{
	int l,m,score = 0;
	for (l = 1; a[i][j+l] == k; l++);
	for (m = 1; a[i][j-m] == k; m++);
	score += score_len(a[i][j-m],a[i][j+l],l+m-1);							//水平方向评分
	
	for (l = 1; a[i+l][j] == k; l++);
	for (m = 1; a[i-m][j] == k; m++);
	score += score_len(a[i-m][j],a[i+l][j],l+m-1);							//垂直方向评分
	
	for (l = 1; a[i+l][j+l] == k; l++);
	for (m = 1; a[i-m][j-m] == k; m++);
	score += score_len(a[i-m][j-m],a[i+l][j+l],l+m-1);						//右斜方向评分
	
	for (l = 1; a[i+l][j-l] == k; l++);
	for (m = 1; a[i-m][j+m] == k; m++);
	score += score_len(a[i-m][j+m],a[i+l][j-l],l+m-1);						//左斜方向评分
		
	x[n].score = score;
}

int score_len(int i,int j,int l)												//评分
{
	if (i == 0 && j == 0)														//两边没有子
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
	else if (i == 3 || j == 3)													//一边为强
	{
		switch (l)
		{
			case 5:
				return 100;
			default:
				return 1;
		}
	}
	else if (i == 0 || j == 0)													//一边没有子
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
	else																			//两边都有子
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
