#include <stdio.h>
#include <signal.h>

int main()
{
	printf("sigkill\n");
	raise(SIGKILL);
	printf("main signal\n");
}
