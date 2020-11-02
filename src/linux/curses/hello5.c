/* hello5.c
 * purpose:bounce a message back and forth across the screen
 * compile cc hello5.c -lcurses -o hello5
 */
//#include <stdio.h>
#include <curses.h>

#define LEFTEDGE  10
#define RIGHTEDGE 30
#define ROW	  10

main()
{
    char *message = "hello";
    char *blank   = "     ";
    int  dir     = +1;
    int  pos     = LEFTEDGE;

    initscr(); //turn on curses, and send requests
    clear();   //clear screen
    while(1)
    {
	move(ROW, pos);
	addstr(message); //draw string
	move(LINES-1, COLS-1); //park the cursor
	refresh();
	sleep(1);
	move(ROW, pos);
	addstr(blank);
	pos += dir;
	if(pos >= RIGHTEDGE)
	    dir = -1;
	if(pos <= LEFTEDGE)
	    dir = +1;
    }
    endwin(); //turn off curses
}
