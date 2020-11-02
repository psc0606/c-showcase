/*----------------------------------------
**	1.先创建文件，然后向文件中写入3个从终端输入的字符串。要求：
**	终端输入顺序 str1,str2,str3
**	写入文件的顺序str1,str2,str3
**	文件内容中实际的存储顺序str1,str3,str2
**----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd;
	long n1, n2, n3;
	char str1[30], str2[30], str3[30];
	pid_t pid;
	if (argc != 2)
	{
		perror("format err");
		exit(0);
	}
	if ((fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		perror("open failed");
		exit(0);
	}
	n1 = read(STDIN_FILENO,str1,sizeof(str1));
	n2 = read(STDIN_FILENO,str2,sizeof(str2));
	n3 = read(STDIN_FILENO,str3,sizeof(str3));
	write(fd,str1,n1);
	lseek(fd,n3,SEEK_CUR);
	write(fd,str2,n2);
	lseek(fd,n1,SEEK_SET);
	write(fd,str3,n3);
}
