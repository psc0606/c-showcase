/* play_agagin2.c
 * purpose:aks if user wants another transaction
 * method:set tty int char-by-char mode(不用回车换行)and no-echo mode(用于忽略非法键)
 *	  read char, return result.
 * returns: 0=>yes, 1=>no
 * better:timeout if user walks away
 */
#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int main()
{
    int response;
    tty_mode(0); //save tty mode
    set_cr_noecho_mode(); //set -icanon, -echo
    response = get_response(QUESTION); //get some another
    tty_mode(1); //restore tty mode
    return response;
}

int get_response(char *question)
{
    int input;
    printf("%s(y/n)?", question);
    while(1)
    {
	switch(input = getchar())
	{
	    case 'y':
	    case 'Y':return 0;
	    case 'n':
	    case 'N':
	    case EOF:
		     return 1;
	    default:
		     printf("\ncannot understand %c, ", input);
		     printf("Please type y or no \n");
	}
    }
}

/*
 * purpose:put file descriptor(i.e. stdin)into char-by-char mode and noecho moe
 * method:use bits in termios
 */
set_cr_noecho_mode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate); //read curr. setting
    ttystate.c_lflag &= ~(ICANON|ECHO); //no buffering, no echo either
    ttystate.c_cc[VMIN] = 1; //get 1 char at a time
    tcsetattr(0, TCSANOW, &ttystate); //install setting
}

tty_mode(int how)
{
    static struct termios original_mode;
    if(how == 0)
	tcgetattr(0, &original_mode);
    else
	return tcsetattr(0, TCSANOW, &original_mode);
}
