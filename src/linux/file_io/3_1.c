/*
 * 如果当前的标准输入流中有输入，偏移成功，否则失败
 * ./a.out < /etc/motd
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <unistd.h>

int main()
{
        if(lseek(STDIN_FILENO, 0, SEEK_CUR) == -1){
	        printf("cannot seek\n");
	}
       	else
            printf("seek ok\n");
	exit(0);
}
