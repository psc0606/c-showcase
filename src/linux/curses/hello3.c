/* hello3.c
 * purpose:using refresh and sleep for animated effects
 * outline:initialize, draw stuff
 */
#include <stdio.h>
#include <curses.h>

main()
{
    int i;

    initscr(); //turn on curses, and send requests
    clear();   //clear screen
    for(i = 0; i < LINES; i++)
    {
	move(i, i+1);
	if(i%2 == 1)
	{
	    standout();
	}
    
	addstr("Hello, world");
	if(i%2 == 1)
	{
	    standend();
	}
	sleep(1);
	refresh(); //update the screen
    }
    endwin(); //turn off curses
}
