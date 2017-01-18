/* 
 * 创建一个带有孔的文件，并用od -c file.hole查看文件内容
 * 用ls -ls file.hole file.nohole查看实际占用磁盘大小 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd;
	char buf1[] = {"aabbccddee"};
	char buf2[] = {"AABBCCDDEE"};

	if((fd = creat("file.hole", 222)) == -1) {
		perror("creat error");
	}
	if(write(fd, buf1, 10) == -1) {
		perror("write buf1 error");	
	}
	/* offset now = 10 */

	if(lseek(fd, 16384, SEEK_SET) == -1){
		perror("lseek error");	
	}
	/* 16384 > size of file.hole, there is a hole in file.hole
	 * offset now = 16384
	 */
	if(write(fd, buf2, 10) == -1) {
		perror("write buf2 error");
	}
	exit(0);
}
