/*sigdemol.c - shows how a signal handler works.
 *run this and press Ctrl-C a few times.
 */
#include <stdio.h>
#include <signal.h>

main()
{
    void f(int); //declare the handler
    int i;
    signal(SIGINT, f);
    for(i = 0; i < 5; i++)
    {
	printf("hello\n");
	sleep(1);
    }
}

void f(int signum)
{
    printf("OUCH! \n");
}
