#include <stdio.h>
#include <signal.h>

int main()
{
	printf("sigalrm\n");
	alarm(1);
	pause();
	printf("main signal\n");
}
