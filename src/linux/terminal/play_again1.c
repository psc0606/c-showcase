/* play_agagin1.c
 * purpose:aks if user wants another transaction
 * method:set tty int char-by-char mode, read char, return result
 * returns: 0=>yes, 1=>no
 * better:do no echo inappropriate input
 */
#include <stdio.h>
#include <termios.h>

#define QUESTION "Do you want another transaction"

int get_response(char *);

int main()
{
    int response;
    tty_mode(0); //save tty mode
    set_crmode();//set char-by-char mode
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
 * purpose:put file descriptor(i.e. stdin)into char-by-char mode
 * method:use bits in termios
 */
set_crmode()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate); //read curr. setting
    ttystate.c_lflag &= ~ICANON; //no buffering
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
