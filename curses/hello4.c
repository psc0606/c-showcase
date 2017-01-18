/* hello4.c
 * purpose:show how to use erae, time, and draw for infomation
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
	move(i, i+i);
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
	move(i, i+i);//move back
	addstr("             "); //erae line
    }
    endwin(); //turn off curses
}
