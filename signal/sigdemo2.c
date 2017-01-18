/*sigdemo2.c - shows how to ignore a signal
 *run this and press Ctrl-\ to kill this one
 */
#include <stdio.h>
#include <signal.h>

main()
{
    void f(int); //declare the handler
    int i;
    signal(SIGINT, SIG_IGN);
    printf("you cannot stop me!\n");
    for(i = 0; i < 5; i++)
    {
	sleep(1);
	printf("haha\n");
    }
}

void f(int signum)
{
    printf("OUCH! \n");
}
