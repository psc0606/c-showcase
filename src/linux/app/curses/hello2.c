/* hello2.c
 * purpose:show the minimal calls needed to use curses
 * outline:initialize, draw stuff, wait for input, quit
 */
#include <stdio.h>
#include <curses.h>

main()
{
    int i;

    initscr(); //turn on curses, and send requests
    clear(); //clear screen
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
    }
    refresh(); //update the screen
    getch(); // wait for user input
    endwin(); //turn off curses
}
